
#include <ncurses.h>
#include <string>
#include "data.h"
#include "windows.h"

static void print_menu(WINDOW *win, int highlight, char const **choices, int n_choices);
void menu_header(string header, const int win_width, const int row=1);
menu_state main_menu(WINDOW *win);
menu_state diff_menu(WINDOW *win);
menu_state cont_menu(WINDOW *win, game_data *data);


/**
 *  Menu for: Game Difficulty selection
 */
menu_state diff_menu(WINDOW *win)
{
    char const *choices[] = {"Easy", "Medium", "Hard"};
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight(1);
    int choice(-999);
    int c, win_y, win_x;

    getmaxyx(stdscr, win_y, win_x);
    init_menu_window(win, win_y, win_x);
    menu_header("May you live in interesting times.", win_x, 3);
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
                menu_header("May you live in interesting times.", win_x, 3);
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
    char const *choices[] = {"Start New", "Quit"};
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight(1);
    int choice(-999);
    int c, win_y, win_x;

    getmaxyx(stdscr, win_y, win_x);
    init_menu_window(win, win_y, win_x);
    menu_header("The Halls of Erebor", win_x);
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
                menu_header("The Halls of Erebor", win_x);
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
 *  Menu: main menu with Continue option
 */
menu_state cont_menu(WINDOW *win, game_data *data)
{
    char const *choices[] = {"Start New", "Continue", "Quit"};
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight(1);
    int choice(-999);
    int c, win_y, win_x;

    getmaxyx(stdscr, win_y, win_x);
    init_menu_window(win, win_y, win_x);
    menu_header("The Halls of Erebor", win_x);
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
                menu_header("The Halls of Erebor", win_x);
                refresh();
                break;
            // no default actions to be taken
        }
        print_menu(win, highlight, choices, n_choices);
        if (choice == 1) {
            data->maze.level = -1;
            return menu_diff;
        } else if (choice == 2) {
            return data->maze.difficulty;
        } else if (choice == 3) {
            return quit;
        }
    }
}


/**
 *  Print the menu header.
 */
void menu_header(string header, const int win_width, const int row) {
    const int str_len(header.length());

    if (str_len > (win_width - 2)) {
        mvprintw(row, 1, header.substr(0, win_width - 2).c_str());
    } else {
        mvprintw(row, (win_width - 2) / 2 - str_len / 2, header.c_str());
    }
}


/**
 *  Print a simple menu window
 */
static void print_menu(WINDOW *win, int highlight, char const **choices, int n_choices)
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

