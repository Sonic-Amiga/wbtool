#include "device.hpp"
#include "menu.hpp"

class ParityMenu : public Menu
{
public:
    ParityMenu(Device &dev);

protected:
    void onCreate(newtComponent form) override;
    int onItemSelected(unsigned int n) override;
	
private:
    Device &device;
};
