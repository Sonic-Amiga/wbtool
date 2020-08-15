#include "device.hpp"

#define WBMR_MAX_INPUTS 7

class WBMR_Device : public DeviceWrapper
{
public:
    WBMR_Device(Device &dev);

    int readInputModes();

    bool hasPowerOffMode() const { return poweroff_mode.has_value(); }
    uint16_t getPowerOffMode() const { return poweroff_mode.value(); }
    bool hasInputMode(int n) const { return input_modes[n].has_value(); }
    uint16_t getInputMode(int n) const { return input_modes[n].value(); }

    int setPowerOffMode(uint16_t mode);
    int setInputMode(int n, uint16_t mode);

private:
    std::optional<uint16_t> poweroff_mode;
    std::optional<uint16_t> input_modes[WBMR_MAX_INPUTS];
};
