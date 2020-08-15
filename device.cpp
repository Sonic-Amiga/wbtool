#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "device.hpp"
#include "wbtool.hpp"

// All (modern) Wirenboard devices have these registers
static const int REG_BAUD     = 110;
static const int REG_PARITY   = 111;
static const int REG_STOPBITS = 112;

Device::Device(const char *port, int baud, char parity, int stop_bit, int address)
    : conn(nullptr), is_connected(false),
      device(port), baud_rate(baud), parity_bit(parity), stop_bits(stop_bit), bus_address(address)
{
    connect();
}

Device::~Device()
{
    disconnect(true);
}

int Device::setBaudRate(int baud)
{
    int rc = writeHoldingReg(REG_BAUD, baud / 100, true);
    
    if (rc == -1)
	return -1;

    disconnect(true);
    baud_rate = baud;
    connect();
    
    return 0;
}

int Device::setParity(int parity)
{
    static const char parity_chars[] = {'N', 'O', 'E'};
    int rc = writeHoldingReg(REG_PARITY, parity, true);
    
    if (rc == -1)
	return -1;

    disconnect(true);
    parity_bit = parity_chars[parity];
    connect();
    
    return 0;
}

int Device::setStopBits(int stop)
{
    int rc = writeHoldingReg(REG_STOPBITS, stop, true);
    
    if (rc == -1)
        return -1;

    disconnect(true);
    stop_bits = stop;
    connect();
    
    return 0;
}

void Device::connect()
{
    int rc;
    char buf[59];

    if (!conn) {
	conn = modbus_new_rtu(device, baud_rate, parity_bit, 8, stop_bits);
	if (!conn)
	    critical("Failed to create modbus context: %s", modbus_strerror(errno));
    }

    if (!is_connected) {
        modbus_set_slave(conn, bus_address);

        if (modbus_connect(conn) == -1)
            critical("Failed to connect to a device: %s", modbus_strerror(errno));
	is_connected = true;
    }

    uptime = readBEInt(104);
    power = readWord(121);
    model = readString(6, 200);
	
    // First try newstyle build number, 2 characters per register
    rc = readRegisters(220, 29, (uint16_t *)buf);
    if (rc == -1) {
	build = readString(22, 220);
    } else {
	buf[58] = 0;
	build = buf;
    }

    version = readString(16, 250);
    extension = readBELong(266);
    serial = readBEInt(270);
    signature = readString(11, 290);
    bootloader = readString(8, 330);
}

void Device::disconnect(bool close_serial)
{
    if (!conn)
	return;

    if (is_connected) {
	modbus_close(conn);
	is_connected = false;
    }

    if (close_serial) {
        modbus_free(conn);
        conn = nullptr;
    }
}

std::optional<std::string> Device::readString(int len, int reg) const
{
    uint16_t *buf = new uint16_t[len];
    int rc = readRegisters(reg, len, buf);
    std::optional<std::string> ret;

    if (rc != -1) {
	std::string str;

	for (int i = 0; i < len && buf[i]; i++)
	    str.push_back(buf[i]);
	ret = str;
    }

    delete[] buf;
    return ret;
}

std::optional<uint16_t> Device::readWord(int reg, bool isInput) const
{
    uint16_t buf;
    int rc = readRegisters(reg, 1, &buf, isInput);

    if (rc == -1) {
	return std::nullopt;
    } else {
	return buf;
    }
}

std::optional<uint32_t> Device::readBEInt(int reg) const
{
    uint16_t buf[2];
    int rc = readRegisters(reg, 2, buf);

    if (rc == -1) {
	return std::nullopt;
    } else {
	return (buf[0] << 16) | buf[1];
    }
}

std::optional<uint64_t> Device::readBELong(int reg) const
{
    uint16_t buf[4];
    int rc = readRegisters(reg, 4, buf);

    if (rc == -1) {
	return std::nullopt;
    } else {
	return ((uint64_t)buf[0] << 48) | ((uint64_t)buf[1] << 32) | (buf[2] << 16) | buf[3];
    }
}

int Device::readRegisters(int reg, int nb, uint16_t *dest, bool isInput) const
{
    int retries = 3;

    do {
        int rc = isInput ? modbus_read_input_registers(conn, reg, nb, dest)
	                 : modbus_read_registers(conn, reg, nb, dest);
	
	if (rc != -1)
	    return rc;

    } while (errno == ETIMEDOUT && --retries);

    if (errno != EMBXILVAL)
	critical("Failed to read %d input registers at %d: %s\n", nb, reg, modbus_strerror(errno));

    return -1;    
}

int Device::writeHoldingReg(int reg, uint16_t data, bool ignore_timeout) const
{
    int retries = 3;

    do {
        int rc = modbus_write_register(conn, reg, data);
	
        if (rc != -1)
            return rc;
    } while (errno == ETIMEDOUT && --retries);

    // This in fact is a kludge in order to attempt to recover from a sutiation
    // when communication port parameter setting has succeeded on the device but
    // the reply has been lost due to a network glitch. In this case all the
    // subsequent retries will time out because e. g. the speed has already been
    // changed, but we'll ignore this and just try to re-establish the communication
    // using new parameters.
    if (errno == ETIMEDOUT && ignore_timeout)
        return 0;

    if (errno != EMBXILVAL)
        critical("Failed to write holding register at %d: %s\n", reg, modbus_strerror(errno));

    return -1;    
}
