#include <stdio.h>
#include <iostream>
#include <ncurses.h>
#include "menu.h"
#include "game.h"

// forward declarations
game_state game_window(WINDOW *menu_win);


/**
 *  Master game FSM
 */
int main()
{
    WINDOW *menu_win;
    game_state state = menu_main;

    initscr();
    clear();
    noecho();
    cbreak();  // Line buffering disabled (use raw() to implement your own cntl-z)
    curs_set(false);

    while (state != quit) {
        if (state == menu_main) {
            state = main_menu(menu_win);
        } else if (state == menu_diff) {
            state = diff_menu(menu_win);
        } else {
            state = easy_ui(menu_win);
        }
    }

    endwin();
    return 0;
}
