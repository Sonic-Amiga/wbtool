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

	rc = readInputRegs(121, 1, &power);
	if (rc == -1)
		power = 0;

    readString(model, 6, 200);
	
	// First try newstyle build number, 2 characters per register
	rc = readInputRegs(220, 29, (uint16_t *)build);
	if (rc == -1) {
		readString(build, 22, 220);
	} else {
		build[58] = 0;
	}

	readString(version, 16, 250);
    extension = readBELong(266);
	serial = readBEInt(270);
	readString(signature, 11, 290);
	readString(bootloader, 8, 330);
}

Device::~Device()
{
	modbus_close(conn);
	modbus_free(conn);
}

void Device::readString(char *p, int len, int reg) const
{
	uint16_t *buf = new uint16_t[len];
	int rc = readInputRegs(reg, len, buf);

	if (rc == -1) {
		strcpy(p, "<N/A>");
	} else {
	    for (int i = 0; i < len; i++)
	        p[i] = buf[i];
	    p[len] = 0;
	}

    delete[] buf;
}

uint32_t Device::readBEInt(int reg) const
{
	uint16_t buf[2];
    int rc = readInputRegs(reg, 2, buf);

	if (rc == -1) {
		return 0;
	} else {
		return (buf[0] << 16) | buf[1];
	}
}

uint64_t Device::readBELong(int reg) const
{
	uint16_t buf[4];
    int rc = readInputRegs(reg, 4, buf);

	if (rc == -1) {
		return 0;
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