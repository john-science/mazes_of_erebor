
#include <random>
#include <ncurses.h>
#include "menu.h"
#include "game.h"
#include "windows.h"


/**
 *  Master game FSM and GUI setup
 */
int main()
{
    WINDOW *win = init_window();
    srand(time(0));

    // master FSM state
    game_state state = menu_main;

    // worlds simplest state machine
    while (state != quit) {
        if (state == menu_main) {
            state = main_menu(win);
        } else if (state == menu_diff) {
            state = diff_menu(win);
        } else if (state == game_medium) {
            state = game_ui_medium(win);
        } else if (state == game_easy || state == game_hard){
            state = game_ui(win, state);
        } else {
            state = quit;
        }
    }

    endwin();
    return 0;
}
