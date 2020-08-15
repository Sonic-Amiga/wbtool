#include "device.hpp"
#include "menu.hpp"

class BaudMenu : public Menu
{
public:
    BaudMenu(Position pos, Device &dev);

protected:
    void onCreate(newtComponent form) override;
    int onItemSelected(unsigned int n) override;
	
private:
    Device &device;
};
