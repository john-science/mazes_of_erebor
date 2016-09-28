
#include <ncurses.h>
#include "windows.h"

/**
 * TODO: build continuable menus
char const *choices_cont[] = {"Start New", "Continue", "Quit",};
int n_choices_cont = sizeof(choices_cont) / sizeof(char *);
*/


enum menu_state {menu_main, menu_diff, menu_cont,
                 game_easy, game_medium, game_hard,
                 too_small, quit};

void print_menu(WINDOW *win, int highlight, char const **choices, int n_choices);
menu_state main_menu(WINDOW *win);
menu_state diff_menu(WINDOW *win);


/**
 *  Menu for: Game Difficulty selection
 */
menu_state diff_menu(WINDOW *win)
{
    char const *choices[] = {"Easy", "Medium", "Hard",};
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight(1);
    int choice(-999);
    int c, win_y, win_x;

    getmaxyx(stdscr, win_y, win_x);
    init_menu_window(win, win_y, win_x);
    mvprintw(3, win_x / 2 - 17, "May you live in interesting times.");
    refresh();

    print_menu(win, highlight, choices, n_choices);
    while (true) {
        // input and update
        c = wgetch(win);
        switch (c) {
            case KEY_UP:
                if(highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if(highlight == n_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10:  // enter key
                choice = highlight;
                break;
            case 410:  // window resize
                getmaxyx(stdscr, win_y, win_x);
                init_menu_window(win, win_y, win_x);
                mvprintw(3, win_x / 2 - 17, "May you live in interesting times.");  // TODO: Move to print menu method?
                refresh();
                break;
            // no default actions to be taken
        }
        // display
        print_menu(win, highlight, choices, n_choices);
        // update
        if (choice == 1) {
            return game_easy;
        } else if (choice == 2) {
            return game_medium;
        } else if (choice == 3) {
            return game_hard;
        }
    }
}


/**
 *  Menu: Default main menu
 */
menu_state main_menu(WINDOW *win)
{
    char const *choices[] = {"Start New", "Quit",};
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight(1);
    int choice(-999);
    int c, win_y, win_x;

    getmaxyx(stdscr, win_y, win_x);
    init_menu_window(win, win_y, win_x);
    mvprintw(1, win_x / 2 - 10, "The Halls of Erebor");  // TODO: Move to print menu method?
    refresh();

    print_menu(win, highlight, choices, n_choices);
    while (true) {
        c = wgetch(win);
        switch (c) {
            case KEY_UP:
                if(highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if(highlight == n_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10:  // enter
                choice = highlight;
                break;
            case 410:  // window resize
                getmaxyx(stdscr, win_y, win_x);
                init_menu_window(win, win_y, win_x);
                mvprintw(1, win_x / 2 - 10, "The Halls of Erebor");
                refresh();
                break;
            // no default actions to be taken
        }
        print_menu(win, highlight, choices, n_choices);
        if (choice == 1) {
            return menu_diff;
        } else if (choice == 2) {
            return quit;
        }
    }
}


/**
 *  Print a simple menu window
 */
void print_menu(WINDOW *win, int highlight, char const **choices, int n_choices)
{
    int i;
    int x(2);
    int y(2);

    box(win, 0, 0);

    for(i = 0; i < n_choices; ++i)
    {
        // Highlight the present choice
        if (highlight == i + 1) {
            wattron(win, A_REVERSE);
            mvwprintw(win, y, x, "%s", choices[i]);
            wattroff(win, A_REVERSE);
        }
        else
            mvwprintw(win, y, x, "%s", choices[i]);
        ++y;
    }
    wrefresh(win);
}

