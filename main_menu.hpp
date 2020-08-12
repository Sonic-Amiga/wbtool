#include <string>

#include "device.hpp"
#include "menu.hpp"

class MainMenu : public Menu
{
public:
    MainMenu(Device &dev);

protected:
    void onCreate(newtComponent form) override;
    int onItemSelected(unsigned int n) override;
	
private:
    void drawHeader();
    void setItemValues();

    Device &device;
    newtComponent header_textbox;
};
