#include "error_popup.hpp"
#include "stop_menu.hpp"
#include "wbtool.hpp"

static const MenuItem choices[] = {
    {"1", 0},
    {"2", 0},
    {"Cancel", 0},
    {NULL, 0}
};

StopMenu::StopMenu(Device &dev) : Menu(17, 19, 0, 0, choices, "Select stop bits"), device(dev)
{
}

void StopMenu::onCreate(newtComponent form)
{
    setCurrentItem(device.getStopBits() - 1);
}

int StopMenu::onItemSelected(unsigned int n)
{
    if (n > 1)
        return MENU_EXIT;

    if (device.setStopBits(n + 1))
	ErrorPopup(17, 19, error_or_unsupported()).Display();

    return 1;
}
