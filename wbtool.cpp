#include "main_menu.hpp"

int main()
{	
	/* Initialize curses */
	initscr();
	start_color();
    cbreak();
    noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);

    {
        MainMenu main_menu;
        main_menu.Execute();
	}

	endwin();
}
