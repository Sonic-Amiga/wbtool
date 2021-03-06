#include "device.hpp"
#include "menu.hpp"

class ParityMenu : public Menu
{
public:
    ParityMenu(Position pos, Device &dev);

protected:
    void onCreate(newtComponent form) override;
    int onItemSelected(unsigned int n) override;
	
private:
    Device &device;
};
