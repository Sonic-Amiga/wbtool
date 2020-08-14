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
    
    MenuItem(const char *t = nullptr, unsigned int vl = 0)
        :title(t), value_length(vl)
    {
    }
};

class Menu
{
public:
    Menu(int y, int x, int width, std::string title = "");
    virtual ~Menu()
    {
    }

    void setItemValue(unsigned int i, const std::string &val);
    int Execute();

protected:
    void init(const MenuItem *choices, int header_h = 0);

    virtual void onCreate(newtComponent form)
    {
    }

    virtual int onItemSelected(unsigned int n)
    {
	return 0;
    }

    int getX() const { return begin_x; }
    int getY() const { return begin_y; }
    int getHeaderHeight() const { return header_height; }
    int getItemWidth() const { return item_width; }
    
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
