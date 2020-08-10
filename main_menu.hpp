#include <string>

#include "device.hpp"
#include "menu.hpp"

class MainMenu : public Menu
{
public:
    MainMenu(Device &dev);

protected:
    void drawHeader(newtComponent textbox) override;
    int onItemSelected(unsigned int n) override;
	
private:
    void setItemValues();

    Device &device;
};
