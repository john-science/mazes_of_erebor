
#include <iostream>
#include <random>
#include <ncurses.h>
#include "data.h"
#include "maze_window.h"
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

    std::cout << "beep 1" << std::endl;

    // create a default game data object
    GameData data;

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
            state = game_loop_maze(win, &data, state);
        } else {
            state = quit;
        }
    }

    endwin();
    return 0;
}
