#include "parity_menu.hpp"

static const MenuItem choices[] = {
    {"None", 0},
    {"Odd", 0},
    {"Even", 0},
    {"Cancel", 0},
    {NULL, 0}
};

ParityMenu::ParityMenu(Device &dev) : Menu(16, 19, 0, 0, choices, "Select parity"), device(dev)
{
}

void ParityMenu::onCreate(newtComponent form)
{
    char parity = device.getParity(); // 'N', 'O', 'E'
    
    for (int item = 0; choices[item].title; item++)
    {
	if (choices[item].title[0] == parity)
	{
	    setCurrentItem(item);
	    break;
	}
    }
}

int ParityMenu::onItemSelected(unsigned int n)
{
    if (n > 2)
        return MENU_EXIT;

    device.setParity(n);
    return 1;
}
