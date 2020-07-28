#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "device.hpp"
#include "wbtool.hpp"

Device::Device(const char *port, int baud, char parity, int stop_bit, int address)
    : baud_rate(baud), parity_bit(parity), stop_bits(stop_bit), bus_address(address)
{
	int rc;
	char buf[59];

	conn = modbus_new_rtu(port, baud, parity, 8, stop_bit);
	if (!conn)
	{
		critical("Failed to create modbus context: %s", modbus_strerror(errno));
	}

	modbus_set_slave(conn, address);
	
    if (modbus_connect(conn) == -1)
	{
		critical("Failed to connect to a device: %s", modbus_strerror(errno));
	}

    uptime = readBEInt(104);
	power = readWord(121);
    model = readString(6, 200);
	
	// First try newstyle build number, 2 characters per register
	rc = readInputRegs(220, 29, (uint16_t *)buf);
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

Device::~Device()
{
	modbus_close(conn);
	modbus_free(conn);
}

// Small hack: actually belongs to MainMenu, but placed here for convenience
int Device::getMenuHeaderSize() const
{
	int s = 0;
	
	s += uptime.has_value();
	s += power.has_value();
	s += model.has_value();
	s += build.has_value();
	s += version.has_value();
	s += extension.has_value();
	s += serial.has_value();
	s += signature.has_value();
	s += bootloader.has_value();
	
	return s;
}

std::optional<std::string> Device::readString(int len, int reg) const
{
	uint16_t *buf = new uint16_t[len];
	int rc = readInputRegs(reg, len, buf);
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

std::optional<uint16_t> Device::readWord(int reg) const
{
	uint16_t buf;
    int rc = readInputRegs(reg, 1, &buf);

	if (rc == -1) {
		return std::nullopt;
	} else {
		return buf;
	}
}

std::optional<uint32_t> Device::readBEInt(int reg) const
{
	uint16_t buf[2];
    int rc = readInputRegs(reg, 2, buf);

	if (rc == -1) {
		return std::nullopt;
	} else {
		return (buf[0] << 16) | buf[1];
	}
}

std::optional<uint64_t> Device::readBELong(int reg) const
{
	uint16_t buf[4];
    int rc = readInputRegs(reg, 4, buf);

	if (rc == -1) {
		return std::nullopt;
	} else {
		return ((uint64_t)buf[0] << 48) | ((uint64_t)buf[1] << 32) | (buf[2] << 16) | buf[3];
	}
}

int Device::readInputRegs(int reg, int nb, uint16_t *dest) const
{
	int retries = 3;

	do {
        int rc = modbus_read_input_registers(conn, reg, nb, dest);
	
	    if (rc != -1)
			return rc;

	} while (errno == ETIMEDOUT && --retries);

    if (errno != EMBXILVAL)
		critical("Failed to read %d input registers at %d: %s\n", nb, reg, modbus_strerror(errno));

    return -1;    
}