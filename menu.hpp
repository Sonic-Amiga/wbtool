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

struct Position
{
    unsigned int x;
    unsigned int y;
};

class Menu
{
public:
    Menu(Position pos, int width, std::string title = "");
    virtual ~Menu()
    {
    }

    void setItemValue(unsigned int i, const std::string &val);
    int Execute();

    Position getSubmenuPosition() const
    {
        // +1 because window border also counts here
        // One more +1 for X coordinate in order to account for space
        // between item title and value
        return {position.x + 2 + item_width,
                position.y + 1 + header_height + getCurrentItem()};
    }

protected:
    void init(const MenuItem *choices, int header_h = 0);

    virtual void onCreate(newtComponent form)
    {
    }

    virtual int onItemSelected(unsigned int n)
    {
        return 0;
    }

    int getHeaderHeight() const { return header_height; }
    int getItemWidth() const { return item_width; }
    
    void setCurrentItem(int n)
    {
        newtListboxSetCurrent(listbox, n);
    }

private:
    unsigned int getCurrentItem() const
    {
        return (unsigned long)newtListboxGetCurrent(listbox);
    }

    std::string formatItem(unsigned int i) const;

    const std::string window_title;
    const struct MenuItem *my_items;
    std::vector<std::string> my_values;
    newtComponent form;
    newtComponent listbox;
    Position position;
    int width;
    int header_height;
    int n_choices;
    unsigned int item_width;
    unsigned int menu_width;
};

#endif
