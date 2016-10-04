
#include <random>
#include <ncurses.h>
#include "data.h"
#include "game.h"
#include "menu.h"
#include "windows.h"


/**
 *  Master game FSM and GUI setup
 */
int main()
{
    WINDOW *win = init_window();
    keypad(win, TRUE);
    srand(time(0));

    // create a default game data object
    game_data data;

    // FSM state
    menu_state state = menu_main;

    // worlds simplest state machine
    while (state != quit) {
        if (state == menu_main) {
            state = main_menu(win);
        } else if (state == menu_diff) {
            state = diff_menu(win);
        } else if (state == menu_cont) {
            state = cont_menu(win, &data);
        } else if (state == game_easy || state == game_hard || state == game_medium){
            state = game_ui(win, &data, state);
        } else {
            state = quit;
        }
    }

    endwin();
    return 0;
}
