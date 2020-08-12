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
	
private:
    void drawHeader();
    void setItemValues();

    Device &device;
    std::vector<MenuItem> items;
    unsigned int cancel_item;
    newtComponent header_textbox;
};
