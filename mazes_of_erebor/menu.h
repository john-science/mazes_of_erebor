
#ifndef MENU_H
#define MENU_H

enum menu_state {menu_main, menu_diff,
                 game_easy, game_medium, game_hard,
                 too_small, quit};

void print_menu(WINDOW *menu_win, int highlight, char const **choices, int n_choices);
menu_state main_menu(WINDOW *menu_win);
menu_state diff_menu(WINDOW *menu_win);

#endif
