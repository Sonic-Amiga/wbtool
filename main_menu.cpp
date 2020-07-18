#include <string.h>

#include "main_menu.hpp"

static const MenuItem choices[] = {
    {"Baud rate", 6},
	{"Parity", 4},
	{"Stop bits", 1},
	{"ModBus Address", 3},
	{"Reboot device", 0},
	{"Exit program", 0},
	{NULL, 0}
};

MainMenu::MainMenu() : Menu(4, 4, 9, 32, choices)
{
	setItemValue(0, "9600");
	setItemValue(1, "none");
	setItemValue(2, "2");
	setItemValue(3, "250");
}

void MainMenu::drawHeader(WINDOW *win)
{
	mvwprintw(win, 1, 2, "Model      : %s", "WB-MRM3");
	mvwprintw(win, 2, 2, "Serial     : %u", 0);
	mvwprintw(win, 3, 2, "Extension  : %llu", 0ULL);
	mvwprintw(win, 4, 2, "Version    : %s", "ver");
	mvwprintw(win, 5, 2, "Build      : %s", "xxx");
	mvwprintw(win, 6, 2, "Signature  : %s", "sig");
	mvwprintw(win, 7, 2, "Bootloader : %s", "BL");
	mvwprintw(win, 8, 2, "Power      : %f", 5.0);
	mvwprintw(win, 9, 2, "Uptime     : %u", 10);
}

int MainMenu::onItemSelected(unsigned int n)
{
	return n == 5;
}
