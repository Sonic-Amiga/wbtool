#include "device.hpp"
#include "menu.hpp"

class StopMenu : public Menu
{
public:
    StopMenu(Device &dev);

protected:
    void onCreate(newtComponent form) override;
    int onItemSelected(unsigned int n) override;
	
private:
    Device &device;
};
