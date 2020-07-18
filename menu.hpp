#include <menu.h>

struct MenuItem
{
	const char *title;
	unsigned int value_length;
};

class Menu
{
public:
    Menu(int y, int x, int header, int w, const MenuItem *choices);
	virtual ~Menu();
	void setItemValue(unsigned int i, const char *val);
	int Execute();

protected:
    virtual void drawHeader(WINDOW *win)
	{
	}

    virtual int onItemSelected(unsigned int n)
	{
		return 0;
	}

private:
	ITEM **my_items;
	char **my_values;
	MENU *my_menu;
	int begin_y;
	int begin_x;
	int width;
	int header_height;
    int n_choices;
    WINDOW *my_menu_win;
};
