#include <sstream>

#include "main_menu.hpp"
#include "baud_menu.hpp"

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
    setItemValues();
}

void MainMenu::setItemValues()
{
    char buffer[7];
    const char *p = NULL;

    sprintf(buffer, "%u", device.getBaudRate());
    setItemValue(0, buffer);

    switch (device.getParity())
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

    sprintf(buffer, "%u", device.getStopBits());
    setItemValue(2, buffer);

    sprintf(buffer, "%u", device.getAddress());
    setItemValue(3, buffer);
}

void MainMenu::drawHeader(newtComponent textbox)
{
    std::stringstream buf;

    if (device.getModel())
	buf << "Model      : " << device.getModel() << '\n';
    if (device.getVersion())
	buf << "Version    : " << device.getVersion() << '\n';
    if (device.getBuild())
	buf << "Build      : " << device.getBuild() << '\n';
    if (device.hasSerial())
	buf << "Serial     : " << device.getSerial() << '\n';
    if (device.hasExtension())
	buf << "Extension  : " << device.getExtension() << '\n';
    if (device.getSignature())
	buf << "Signature  : " << device.getSignature() << '\n';
    if (device.getBootloader())
	buf << "Bootloader : " << device.getBootloader() << '\n';
    if (device.hasPower())
	buf << "Power      : " << device.getPower() / 1000.0 << " V\n";
    if (device.hasUptime())
	buf << "Uptime     : " << device.getUptime() << " sec\n";

    newtTextboxSetText(textbox, buf.str().c_str());
}

int MainMenu::onItemSelected(unsigned int n)
{
    int rc;

    if (n == 0) {
        rc = BaudMenu(device).Execute();
    } else if (n == 5) {
	return MENU_EXIT;
    }

    if (rc != MENU_EXIT) {
	drawHeader(getHeaderBox());
        setItemValues();
    }

    return 0;
}
