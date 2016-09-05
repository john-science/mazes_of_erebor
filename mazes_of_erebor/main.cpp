
#include <ncurses.h>
#include "menu.h"
#include "game.h"


/**
 *  Master game FSM and GUI setup
 */
int main()
{
    WINDOW *menu_win;
    game_state state = menu_main;

    // init NCURSES GUI
    initscr();
    clear();
    noecho();              // disable echo text
    cbreak();              // disable line buffering
    curs_set(false);       // disable cursor
    keypad(stdscr, true);  // enable key pad

    // master game FSM
    while (state != quit) {
        if (state == menu_main) {
            state = main_menu(menu_win);  // TODO: Warning: menu_win un-init
        } else if (state == menu_diff) {
            state = diff_menu(menu_win);
        } else {
            state = easy_ui(menu_win);
        }
    }

    endwin();
    return 0;
}
