#include <string>

#include "menu.hpp"

class MainMenu : public Menu
{
public:
    MainMenu();

protected:
	void drawHeader(WINDOW *win) override;
	int onItemSelected(unsigned int n) override;
	
private:
    char str_baud[7];
	char str_parity[5];
	char str_stop[2];
	char str_address[4];
};
