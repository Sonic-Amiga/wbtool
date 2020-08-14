#include <sstream>

#include "wbmr_menu.hpp"

static const char *PowerOffModes[] =
{
    "Restore last state",
    "All off"
};

static const char *InputModes[] =
{
    "Pushbutton",
    "Rocker switch",
    "All off",
    "No control",
    "Mapping matrix",
    "Mapping matrix with repeat"
};

static std::string _stringForNumber(uint16_t n, const char **table, size_t table_size)
{
    if (n >= table_size / sizeof(const char *)) {
	std::stringstream s;

	s << "Unknown (" << n << ')';
	return s.str();
    } else {
	return table[n];
    }
}

#define stringForIdx(n, t) _stringForNumber(n, t, sizeof(t))

const MenuItem WBMR_MainMenu::choices[] =
{
    {"Power off handling", 18},
    {"Input modes", 0},
    {NULL, 0}
};

WBMR_MainMenu::WBMR_MainMenu(WBMR_Device &dev) : MainMenu(dev.getDevice(), choices), device(dev)
{
    setItemValues();
}

void WBMR_MainMenu::setItemValues()
{
    if (device.hasPowerOffMode()) {
        unsigned int base = getFirstDeviceItemIndex();
        uint16_t poweroff_mode = device.getPowerOffMode();

        setItemValue(base, stringForIdx(poweroff_mode, PowerOffModes));
    }
}

int WBMR_MainMenu::onItemSelected(unsigned int n)
{
    unsigned int base = getFirstDeviceItemIndex();
	    
    if (n == base) {
        // TODO: Power off handling
    } else if (n == base + 1) {
	device.readInputModes();
	WBMR_InputModeMenu(device, getX() + getItemWidth(), getY() + getHeaderHeight() + 7).Execute();
    } else {
	return MainMenu::onItemSelected(n);
    }

    return 0;
}


const MenuItem WBMR_InputModeMenu::choices[] =
{
    {"Input #1", 26},
    {"Input #2", 26},
    {"Input #3", 26},
    {"Input #4", 26},
    {"Input #5", 26},
    {"Input #6", 26},
    {"Input #0", 26},
};

WBMR_InputModeMenu::WBMR_InputModeMenu(WBMR_Device &dev, int x, int y)
    : Menu(y, x, 0, "WB-MR input modes"), device(dev), input_count(0)
{
    for (int i = 0; i < WBMR_MAX_INPUTS; i++) {
	if (dev.hasInputState(i)) {	
	    input_map[input_count++] = i;
	    items.push_back(choices[i]);
	}
    }

    items.push_back(MenuItem("Return"));
    items.push_back(MenuItem());

    init(&items[0]);
    setItemValues();
}

void WBMR_InputModeMenu::setItemValues()
{
    for (unsigned int i = 0; i < input_count; i++) {
	uint16_t mode = device.getInputState(input_map[i]);

        setItemValue(i, stringForIdx(mode, InputModes));
    }
}

int WBMR_InputModeMenu::onItemSelected(unsigned int n)
{
    if (n >= input_count) {
        return MENU_EXIT;
    } else {
	return 0;
    }
}