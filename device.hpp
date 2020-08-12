#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <modbus.h>
#include <optional>

static inline const char *c_str(const std::optional<std::string> &str)
{
    return str.has_value() ? str.value().c_str() : nullptr;
}

class Device
{
public:
    Device(const char *device, int baud, char parity, int stop_bit, int address);
    ~Device();

    int getBaudRate() const {return baud_rate; }
    char getParity() const {return parity_bit; }
    int getStopBits() const {return stop_bits; }
    int getAddress() const {return bus_address; }
    bool hasUptime() const {return uptime.has_value(); }
    uint32_t getUptime() const {return uptime.value(); }
    bool hasPower() const {return power.has_value(); }
    uint16_t getPower() const {return power.value(); }
    const char *getModel() const {return c_str(model); }
    const char *getBuild() const {return c_str(build); }
    const char *getVersion() const {return c_str(version); }
    bool hasExtension() const {return extension.has_value(); }
    uint64_t getExtension() const {return extension.value(); }
    bool hasSerial() const {return serial.has_value(); }
    uint32_t getSerial() const {return serial.value(); }
    const char *getSignature() const {return c_str(signature); }
    const char *getBootloader() const {return c_str(bootloader); }

    int getMenuHeaderSize() const;
    
    int setBaudRate(int baud);
    int setParity(int parity);
    int setStopBits(int stop);

private:
    void connect();
    void disconnect(bool close_serial = false);
    std::optional<std::string> readString(int len, int reg) const;
    std::optional<uint16_t> readWord(int reg) const;
    std::optional<uint32_t> readBEInt(int reg) const;
    std::optional<uint64_t> readBELong(int reg) const;
    int readInputRegs(int addr, int nb, uint16_t *dest) const;
    int writeHoldingReg(int reg, uint16_t data, bool ignore_timeout = false) const;

    modbus_t *conn;
    bool is_connected;
    const char *device;
    int baud_rate;
    char parity_bit;
    int stop_bits;
    int bus_address;
    std::optional<uint32_t> uptime;
    std::optional<uint16_t> power;
    std::optional<std::string> model;
    std::optional<std::string> build;
    std::optional<std::string> version;
    std::optional<uint64_t> extension;
    std::optional<uint32_t> serial;
    std::optional<std::string> signature;
    std::optional<std::string> bootloader;
};

#endif
