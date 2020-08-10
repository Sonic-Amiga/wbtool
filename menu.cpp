#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include "menu.hpp"

Menu::Menu(int y, int x, int header_h, int w, const MenuItem *choices)
    : my_items(choices), begin_y(y), begin_x(x), width(w), header_height(header_h),
      item_width(0), value_width(0)
{
    for (n_choices = 0; choices[n_choices].title; n_choices++)
    {
	size_t l = strlen(choices[n_choices].title);
	
	if (l > item_width)
	    item_width = l;
        if (choices[n_choices].value_length > value_width)
	    value_width = choices[n_choices].value_length;
    }

    my_values.resize(n_choices);

    if (header_height)
	header_height += 1; // Reserve space for separator
}

void Menu::setItemValue(unsigned int i, const char *val)
{
    my_values[i] = val;
    
    if (listbox) {
        newtListboxSetEntry(listbox, i, formatItem(i).c_str());
    }
}

std::string Menu::formatItem(unsigned int i) const
{
    std::string buf = my_items[i].title;

    buf.append(item_width - buf.size() + 1, ' ');
    buf.append(my_values[i]);
    
    return buf;
}

int Menu::Execute()
{
    int rc = 0;

    newtOpenWindow(begin_x, begin_y, width, header_height + n_choices, nullptr);
    form = newtForm(NULL, NULL, 0);

    if (header_height)
    {
        header_textbox = newtTextbox(0, 0, width, header_height, NEWT_FLAG_WRAP);
	drawHeader(header_textbox);

/*	mvwaddch(my_menu_win, header_height, 0, ACS_LTEE);
	mvwhline(my_menu_win, header_height, 1, ACS_HLINE, width);
	mvwaddch(my_menu_win, header_height, width + 1, ACS_RTEE);*/
	newtFormAddComponent(form, header_textbox);
    }

    listbox = newtListbox(0, header_height, n_choices, NEWT_FLAG_RETURNEXIT);
    
    for (size_t i = 0; i < n_choices; i++) {
        newtListboxAppendEntry(listbox, formatItem(i).c_str(), (void *)i);
    }

    newtFormAddComponent(form, listbox);

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