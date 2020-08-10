#include "baud_menu.hpp"

static const MenuItem choices[] = {
    {"1200", 0},
    {"2400", 0},
    {"9600", 0},
    {"19200", 0},
    {"38400", 0},
    {"57600", 0},
    {"115200", 0},
    {"Cancel", 0},
    {NULL, 0}
};

static const int rates[] = {
    1200,
    2400,
    9600,
    19200,
    38400,
    57600,
    115200
};

BaudMenu::BaudMenu(Device &dev) : Menu(6, 6, 1, 16, choices), device(dev)
{
}

void BaudMenu::drawHeader(newtComponent textbox)
{
    newtTextboxSetText(textbox, "Select baud rate");
}

int BaudMenu::onItemSelected(unsigned int n)
{
    if (n > 6)
        return MENU_EXIT;

    device.setBaudRate(rates[n]);
    return 1;
}
