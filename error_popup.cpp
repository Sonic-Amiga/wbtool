#include <string.h>

#include "error_popup.hpp"

static const char *header = "Error";
static const int x_space = 4;
static const int y_space = 1;
static const char *footer = "Press any key";

ErrorPopup::ErrorPopup(int y, int x, std::string body)
    :begin_x(x), begin_y(y), message(body)
{
    int l = 0;

    width = strlen(footer);
    height = 1;

    for (size_t i = 0; i < body.size(); i++)
    {
        if (body[i] == '\n') {
	    height++;
	    if (l > width)
	        width = l;
	    l = 0;
	} else {
	    l++;
	}
    }

    if (l > width)
        width = l;
}

void ErrorPopup::Display()
{
    int full_w = width + x_space * 2;
    int full_h = height + y_space * 2;
    int footer_w = strlen(footer);
    int footer_x = (full_w - footer_w) / 2;

    newtOpenWindow(begin_x, begin_y, full_w, full_h + 1, header);

    newtComponent form = newtForm(NULL, NULL, 0);

    newtComponent textbox = newtTextbox(x_space, y_space, width, height, NEWT_FLAG_WRAP);
    newtTextboxSetText(textbox, message.c_str());

    newtComponent footerbox = newtTextbox(footer_x, full_h, footer_w, 1, NEWT_FLAG_WRAP);
    newtTextboxSetText(footerbox, footer);

    newtFormAddComponents(form, textbox, footerbox, nullptr);

    newtDrawForm(form);
    newtWaitForKey();

    newtFormDestroy(form);
    newtPopWindow();
}