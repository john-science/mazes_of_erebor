
#ifndef MENU_H
#define MENU_H

#include "data.h"

void print_menu(WINDOW *menu_win, int highlight, char const **choices, int n_choices);
menu_state main_menu(WINDOW *menu_win);
menu_state diff_menu(WINDOW *menu_win);
menu_state cont_menu(WINDOW *win, game_data *data);

#endif
