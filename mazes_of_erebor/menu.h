
#ifndef MENU_H
#define MENU_H

enum game_state {menu_main, menu_diff,
                 game_easy, game_medium, game_hard, game_intro, game_splash,
                 quit};

void print_menu(WINDOW *menu_win, int highlight, char const **choices, int n_choices);
game_state main_menu(WINDOW *menu_win);
game_state diff_menu(WINDOW *menu_win);

#endif
