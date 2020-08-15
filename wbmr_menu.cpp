#include <sstream>

#include "error_popup.hpp"
#include "wbmr_menu.hpp"
#include "wbtool.hpp"

static const MenuItem PowerOffModes[] =
{
    {"Restore last state", 0},
    {"All off", 0},
    {"Cancel", 0},
    {nullptr, 0}
};

static const MenuItem InputModes[] =
{
    {"Pushbutton", 0},
    {"Rocker switch", 0},
    {"All off", 0},
    {"No control", 0},
    {"Mapping matrix", 0},
    {"Mapping matrix with repeat", 0},
    {"Cancel", 0},
    {nullptr, 0}
};

static std::string _stringForNumber(uint16_t n, const MenuItem *table, size_t table_size)
{
    // Drop "Cancel" and terminator
    if (n >= table_size / sizeof(MenuItem) - 2) {
        std::stringstream s;

        s << "Unknown (" << n << ')';
        return s.str();
    } else {
        return table[n].title;
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
    int rc;
	    
    if (n == base) {
        rc = WBMR_PowerOffModeMenu(getSubmenuPosition(), device).Execute();
    } else if (n == base + 1) {
        device.readInputModes();
        WBMR_InputModeMenu(getSubmenuPosition(), device).Execute();
        rc = MENU_EXIT; // We do not display any values of these
    } else {
        return MainMenu::onItemSelected(n);
    }

    if (rc != MENU_EXIT)
        setItemValues();

    return 0;
}

WBMR_PowerOffModeMenu::WBMR_PowerOffModeMenu(Position pos, WBMR_Device &dev)
    : Menu(pos, 0, "Select power off handling"), device(dev)
{
    init(PowerOffModes);
}

void WBMR_PowerOffModeMenu::onCreate(newtComponent form)
{
    uint16_t mode = device.getPowerOffMode();

    setCurrentItem(mode);
}

int WBMR_PowerOffModeMenu::onItemSelected(unsigned int n)
{
    if (n > 1)
        return MENU_EXIT;

    if (device.setPowerOffMode(n) != 0)
        ErrorPopup(16, 19, error_or_unsupported()).Display();

    return 1;
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

WBMR_InputModeMenu::WBMR_InputModeMenu(Position pos, WBMR_Device &dev)
    : Menu(pos, 0, "WB-MR input modes"), device(dev), input_count(0)
{
    for (int i = 0; i < WBMR_MAX_INPUTS; i++) {
        if (dev.hasInputMode(i)) {	
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
        uint16_t mode = device.getInputMode(input_map[i]);

        setItemValue(i, stringForIdx(mode, InputModes));
    }
}

int WBMR_InputModeMenu::onItemSelected(unsigned int n)
{
    int rc = MENU_EXIT;

    if (n >= input_count) {
        return MENU_EXIT;
    } else {
        rc = InputModeChangeMenu(getSubmenuPosition(), input_map[n], device).Execute();
    }

    if (rc != MENU_EXIT)
        setItemValues();

    return 0;
}

InputModeChangeMenu::InputModeChangeMenu(Position pos, unsigned int idx, WBMR_Device &dev)
    : Menu(pos, 0, "Select input mode"), device(dev), input_index(idx)
{
    init(InputModes);
}

void InputModeChangeMenu::onCreate(newtComponent form)
{
    uint16_t mode = device.getInputMode(input_index);

    setCurrentItem(mode);
}

int InputModeChangeMenu::onItemSelected(unsigned int n)
{
    if (n > 5)
        return MENU_EXIT;

    if (device.setInputMode(input_index, n) != 0)
        ErrorPopup(16, 19, error_or_unsupported()).Display();

    return 1;
}
