#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include <string>

#include "device.hpp"
#include "menu.hpp"

class MainMenu : public Menu
{
public:
    MainMenu(Device &dev, const MenuItem *choices = nullptr);

protected:
    void init(const MenuItem *choices);
    void onCreate(newtComponent form) override;
    int onItemSelected(unsigned int n) override;
    virtual void deviceItemSelected(unsigned int n)
    {
    }

    unsigned int getFirstDeviceItemIndex() const { return common_items_count; }

    Device &device;

private:
    void drawHeader();
    void setItemValues();

    std::vector<MenuItem> items;
    unsigned int common_items_count;
    unsigned int cancel_item;
    newtComponent header_textbox;
};

#endif
