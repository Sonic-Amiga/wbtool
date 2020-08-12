#include <sstream>

#include "main_menu.hpp"
#include "baud_menu.hpp"
#include "parity_menu.hpp"
#include "stop_menu.hpp"

static const MenuItem common_choices[] = {
    {"Baud rate", 6},
    {"Parity", 4},
    {"Stop bits", 1},
    {"ModBus Address", 3},
    {"Reboot device", 0},
    {NULL, 0}
};

static const unsigned int WINDOW_WIDTH = 72;

MainMenu::MainMenu(Device &dev, const MenuItem *choices)
    : Menu(4, 4, WINDOW_WIDTH), device(dev)
{   
    init(choices);
}

void MainMenu::init(const MenuItem *choices)
{
    int s = 0;

    s += device.hasUptime();
    s += device.hasPower();
    s += !!device.getModel();
    s += !!device.getBuild();
    s += !!device.getVersion();
    s += device.hasExtension();
    s += device.hasSerial();
    s += !!device.getSignature();
    s += !!device.getBootloader();

    for (int i = 0; common_choices[i].title; i++)
        items.push_back(common_choices[i]);

    if (choices) {
        for (int i = 0; choices[i].title; i++)
            items.push_back(choices[i]);
    }

    cancel_item = items.size();

    items.push_back(MenuItem("Exit program"));
    items.push_back(MenuItem());
    
    Menu::init(&items[0], s);
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

void MainMenu::onCreate(newtComponent form)
{
    header_textbox = newtTextbox(0, 0, WINDOW_WIDTH, getHeaderHeight(), NEWT_FLAG_WRAP);
    drawHeader();

    newtFormAddComponent(form, header_textbox);
}

void MainMenu::drawHeader()
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

    newtTextboxSetText(header_textbox, buf.str().c_str());
}

int MainMenu::onItemSelected(unsigned int n)
{
    int rc;

    if (n == 0) {
        rc = BaudMenu(device).Execute();
    } else if (n == 1) {
	rc = ParityMenu(device).Execute();
    } else if (n == 2) {
	rc = StopMenu(device).Execute();
    } else if (n == cancel_item) {
	return MENU_EXIT;
    }

    if (rc != MENU_EXIT) {
	drawHeader();
        setItemValues();
    }

    return 0;
}
