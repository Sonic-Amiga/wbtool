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

MainMenu::MainMenu(Device &dev) : Menu(4, 4, dev.getMenuHeaderSize(), 72, choices), device(dev)
{
	char buffer[7];
	const char *p = NULL;
	
	sprintf(buffer, "%u", dev.getBaudRate());
	setItemValue(0, buffer);
	
	switch (dev.getParity())
	{
	case 'N':
	    p = "none";
	    break;
	case 'E':
	    p = "even";
		break;
	case 'O':
	    p = "odd";
		break;
	}
	setItemValue(1, p);
	
	sprintf(buffer, "%u", dev.getStopBits());
	setItemValue(2, buffer);

	sprintf(buffer, "%u", dev.getAddress());
	setItemValue(3, buffer);
}

void MainMenu::drawHeader(WINDOW *win)
{
	int l = 1;

    if (device.getModel())
	    mvwprintw(win, l++, 2, "Model      : %s", device.getModel());
	if (device.getVersion())
	    mvwprintw(win, l++, 2, "Version    : %s", device.getVersion());
	if (device.getBuild())
	    mvwprintw(win, l++, 2, "Build      : %s", device.getBuild());
	if (device.hasSerial())
		mvwprintw(win, l++, 2, "Serial     : %u", device.getSerial());
	if (device.hasExtension())
	    mvwprintw(win, l++, 2, "Extension  : %llu", device.getExtension());
	if (device.getSignature())
	    mvwprintw(win, l++, 2, "Signature  : %s", device.getSignature());
	if (device.getBootloader())
	    mvwprintw(win, l++, 2, "Bootloader : %s", device.getBootloader());
	if (device.hasPower())
	    mvwprintw(win, l++, 2, "Power      : %.1f V", device.getPower() / 1000.0);
	if (device.hasUptime())
	    mvwprintw(win, l++, 2, "Uptime     : %u sec", device.getUptime());
}

int MainMenu::onItemSelected(unsigned int n)
{
	return n == 5;
}
