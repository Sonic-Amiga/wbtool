#include "device.hpp"
#include "menu.hpp"

class BaudMenu : public Menu
{
public:
    BaudMenu(Device &dev);

protected:
	void drawHeader(WINDOW *win) override;
	int onItemSelected(unsigned int n) override;
	
private:
    Device &device;
};
