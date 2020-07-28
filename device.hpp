#include <modbus.h>

class Device
{
public:
    Device(const char *device, int baud, char parity, int stop_bit, int address);
	~Device();

    int getBaudRate() const {return baud_rate; }
	char getParity() const {return parity_bit; }
	int getStopBits() const {return stop_bits; }
	int getAddress() const {return bus_address; }
	uint32_t getUptime() const {return uptime; }
	uint16_t getPower() const {return power; }
	const char *getModel() const {return model; }
	const char *getBuild() const {return build; }
	const char *getVersion() const {return version; }
	uint64_t getExtension() const {return extension; }
	uint32_t getSerial() const {return serial; }
	const char *getSignature() const {return signature; }
	const char *getBootloader() const {return bootloader; }

private:
    void readString(char *p, int len, int reg) const;
	uint32_t readBEInt(int reg) const;
	uint64_t readBELong(int reg) const;

    modbus_t *conn;
	int baud_rate;
	char parity_bit;
	int stop_bits;
	int bus_address;
	uint32_t uptime;
	uint16_t power;
	char model[7];
	char build[59];
	char version[17];
	uint64_t extension;
	uint32_t serial;
	char signature[12];
	char bootloader[9];
};
