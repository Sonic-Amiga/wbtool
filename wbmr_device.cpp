#include "wbmr_device.hpp"

static const int WBMR_REG_POWER_OFF_MODE = 6;
static const int WBMR_REG_INPUT_1_MODE = 9;
static const int WBMR_REG_INPUT_0_MODE = 16;

WBMR_Device::WBMR_Device(Device &dev) : DeviceWrapper(dev)
{
    poweroff_mode = device.readWord(WBMR_REG_POWER_OFF_MODE, false);
}

int WBMR_Device::readInputModes()
{
    int rc = -1;

    for (int i = 0; i < WBMR_MAX_INPUTS; i++) {
	input_modes[i] = device.readWord(WBMR_REG_INPUT_1_MODE + i, false);
	if (input_modes[i].has_value())
	    rc = 0;
    }
    
    return rc;
}
