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
    Menu(int y, int x, int header_height, int width, const MenuItem *choices, std::string title = "");
    virtual ~Menu()
    {
    }

    void setItemValue(unsigned int i, const char *val);
    int Execute();

protected:
    virtual void onCreate(newtComponent form)
    {
    }

    virtual int onItemSelected(unsigned int n)
    {
	return 0;
    }

    int getHeaderHeight() const { return header_height; }
    
    void setCurrentItem(int n)
    {
        newtListboxSetCurrent(listbox, n);
    }

private:
    std::string formatItem(unsigned int i) const;

    const std::string window_title;
    const struct MenuItem *my_items;
    std::vector<std::string> my_values;
    newtComponent form;
    newtComponent listbox;
    int begin_y;
    int begin_x;
    int width;
    int header_height;
    int n_choices;
    size_t item_width;
    size_t menu_width;
};

#endif
