#include "error_popup.hpp"
#include "parity_menu.hpp"
#include "wbtool.hpp"

static const MenuItem choices[] = {
    {"None", 0},
    {"Odd", 0},
    {"Even", 0},
    {"Cancel", 0},
    {NULL, 0}
};

ParityMenu::ParityMenu(Device &dev) : Menu(16, 19, 0, "Select parity"), device(dev)
{
    init(choices);
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

    if (device.setParity(n) != 0)
	ErrorPopup(16, 19, error_or_unsupported()).Display();   

    return 1;
}
