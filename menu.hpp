#ifndef MENU_HPP
#define MENU_HPP

#include <newt.h>

#include <string>
#include <vector>

// Common exit codes
#define MENU_EXIT -1

struct MenuItem
{
    const char *title;
    unsigned int value_length;
};

class Menu
{
public:
    Menu(int y, int x, int header, int w, const MenuItem *choices);
    virtual ~Menu()
    {
    }

    void setItemValue(unsigned int i, const char *val);
    int Execute();

protected:
    virtual void drawHeader(newtComponent textbox)
    {
    }

    virtual int onItemSelected(unsigned int n)
    {
	return 0;
    }

    newtComponent getHeaderBox() const {return header_textbox;}

private:
    std::string formatItem(unsigned int i) const;

    const struct MenuItem *my_items;
    std::vector<std::string> my_values;
    newtComponent form;
    newtComponent header_textbox;
    newtComponent listbox;
    int begin_y;
    int begin_x;
    int width;
    int header_height;
    int n_choices;
    size_t item_width;
    size_t value_width;
};

#endif
