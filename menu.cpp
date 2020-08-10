#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include "menu.hpp"

Menu::Menu(int y, int x, int header_h, int w, const MenuItem *choices)
    : begin_y(y), begin_x(x), width(w), header_height(header_h)
{
    for (n_choices = 0; choices[n_choices].title; n_choices++);

    my_items = new ITEM *[n_choices + 1];
    my_values = new char *[n_choices];

    for (size_t i = 0; i < n_choices; ++i)
    {
	int l = choices[i].value_length;

        if (l)
        {
	    char *buf = new char[l + 1];

	    my_values[i] = buf;
	    memset(buf, ' ', l);
	    buf[l] = 0;
        }
        else
        {
	    my_values[i] = NULL;
        }
        my_items[i] = new_item(choices[i].title, my_values[i]);
        set_item_userptr(my_items[i], (void *)i);
    }
    my_items[n_choices] = nullptr; // Terminate entries array

    if (header_height)
	header_height += 1; // Reserve space for separator

    my_menu = new_menu(my_items);
}

Menu::~Menu()
{
    free_menu(my_menu);
    for (int i = 0; i < n_choices; ++i)
    {
        free_item(my_items[i]);
	if (my_values[i])
	   delete[] my_values[i];
    }
    delete[] my_items;
    delete[] my_values;
}

void Menu::setItemValue(unsigned int i, const char *val)
{
    char *buf = my_values[i];

    if (buf) {
	int l = strlen(val);

	memcpy(my_values[i], val, l);
	if (l < my_items[i]->description.length)
	    memset(&buf[l], ' ', my_items[i]->description.length - l);
    }
}

int Menu::Execute()
{
    int rc = 0;

    /* Create the window to be associated with the menu */
    my_menu_win = newwin(header_height + n_choices + 2, width + 2, begin_y, begin_x);
    keypad(my_menu_win, TRUE);
     
    /* Set main window and sub window */
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, n_choices, width, header_height + 1, 1));

    /* Print a border around the main window and print a title */
    box(my_menu_win, 0, 0);

    if (header_height)
    {
	drawHeader(my_menu_win);

	mvwaddch(my_menu_win, header_height, 0, ACS_LTEE);
	mvwhline(my_menu_win, header_height, 1, ACS_HLINE, width);
	mvwaddch(my_menu_win, header_height, width + 1, ACS_RTEE);
    }

    refresh();

    post_menu(my_menu);

    do
    {
	int c;

        wrefresh(my_menu_win);
	c = wgetch(my_menu_win);
	switch(c)
	{
	case KEY_DOWN:
	    menu_driver(my_menu, REQ_DOWN_ITEM);
	    break;
	case KEY_UP:
	    menu_driver(my_menu, REQ_UP_ITEM);
	    break;
	case '\n':
	    rc = onItemSelected((unsigned long)item_userptr(current_item(my_menu)));
	    redrawwin(my_menu_win);
	    break;
	}
    } while (rc == 0);

    unpost_menu(my_menu);
    return rc;
}