#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include "menu.hpp"

Menu::Menu(int y, int x, int w, std::string title)
    : window_title(title), listbox(nullptr), begin_y(y), begin_x(x), width(w), item_width(0)
{
    // Four characters around title: two spaces and framing
    size_t header_width = title.size() + 4;
    
    if (width < header_width)
	width = header_width;    
}

// Some classes may want to fill in "choices" dynamically. It's not convenient
// to do so in a static method before calling parent constructor, so this
// "post-constructor" is separated.
void Menu::init(const MenuItem *choices, int header_h)
{
    size_t value_width = 0;

    header_height = header_h;
    if (header_height)
	header_height += 1; // Reserve space for separator

    for (n_choices = 0; choices[n_choices].title; n_choices++)
    {
	size_t l = strlen(choices[n_choices].title);
	
	if (l > item_width)
	    item_width = l;
        if (choices[n_choices].value_length > value_width)
	    value_width = choices[n_choices].value_length;
    }

    menu_width = item_width;
    if (value_width)
        menu_width += value_width + 1; // Space between name and value
 
    if (width < menu_width)
	width = menu_width;

    my_items = choices;
    my_values.resize(n_choices);
}

void Menu::setItemValue(unsigned int i, const std::string &val)
{
    my_values[i] = val;
    
    if (listbox) {
        newtListboxSetEntry(listbox, i, formatItem(i).c_str());
    }
}

std::string Menu::formatItem(unsigned int i) const
{
    std::string buf = my_items[i].title;

    buf.append(item_width - buf.size(), ' ');

    if (menu_width != item_width) {
	buf.append(" ");
        buf.append(my_values[i]);
        buf.append(menu_width - buf.size(), ' ');
    }

    return buf;
}

int Menu::Execute()
{
    int rc = 0;

    newtOpenWindow(begin_x, begin_y, width, header_height + n_choices, window_title.empty() ? nullptr : window_title.c_str());
    form = newtForm(NULL, NULL, 0);
    listbox = newtListbox(0, header_height, n_choices, NEWT_FLAG_RETURNEXIT);
    
    for (size_t i = 0; i < n_choices; i++) {
        newtListboxAppendEntry(listbox, formatItem(i).c_str(), (void *)i);
    }

    newtFormAddComponent(form, listbox);

    onCreate(form);

    do
    {
	newtComponent choice = newtRunForm(form);
	
	if (choice) {
	    rc = onItemSelected((unsigned long)newtListboxGetCurrent(listbox));
	} else {
	    rc = MENU_EXIT;
	}
    } while (rc == 0);

    listbox = nullptr;
    newtFormDestroy(form);
    newtPopWindow();

    return rc;
}