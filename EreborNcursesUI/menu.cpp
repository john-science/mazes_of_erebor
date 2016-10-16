
#include <ncurses.h>
#include <string>
#include <string.h>
#include "data.h"
#include "windows.h"

static void print_menu(WINDOW *win, const int highlight, char const **choices, const int n_choices, const int win_y, const int win_x, string header);
static void menu_header(WINDOW *win, string header, const int win_width, const int row=1);
menu_state main_menu(WINDOW *win);
menu_state diff_menu(WINDOW *win);
menu_state cont_menu(WINDOW *win, GameData *data);


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
    print_menu(win, highlight, choices, n_choices, win_y, win_x, "May you live in interesting times.");

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
                full_box_resize(win, win_y, win_x);
                break;
            // no default actions to be taken
        }
        // display
        print_menu(win, highlight, choices, n_choices, win_y, win_x, "May you live in interesting times.");
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
    print_menu(win, highlight, choices, n_choices, win_y, win_x, "The Halls of Erebor");

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
                full_box_resize(win, win_y, win_x);
                break;
            // no default actions to be taken
        }
        print_menu(win, highlight, choices, n_choices, win_y, win_x, "The Halls of Erebor");
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
menu_state cont_menu(WINDOW *win, GameData *data)
{
    char const *choices[] = {"Start New", "Continue", "Quit"};
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight(1);
    int choice(-999);
    int c, win_y, win_x;

    getmaxyx(stdscr, win_y, win_x);
    print_menu(win, highlight, choices, n_choices, win_y, win_x, "The Halls of Erebor");

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
                full_box_resize(win, win_y, win_x);
                break;
            // no default actions to be taken
        }
        print_menu(win, highlight, choices, n_choices, win_y, win_x, "The Halls of Erebor");
        if (choice == 1) {
            data->maze.level = -1;
            return menu_diff;
        } else if (choice == 2) {
            return data->difficulty;
        } else if (choice == 3) {
            return quit;
        }
    }
}


/**
 *  Print the menu header.
 */
static void menu_header(WINDOW *win, string header, const int win_width, const int row) {
    const int str_len(header.length());

    if (str_len > (win_width - 2)) {
        mvwprintw(win, row, 1, header.substr(0, win_width - 2).c_str());
    } else {
        mvwprintw(win, row, (win_width - 2) / 2 - str_len / 2, header.c_str());
    }
}


// TODO: Too many arguments. This should be in a class.
/**
 *  Print a simple menu window
 */
static void print_menu(WINDOW *win, const int highlight, char const **choices, const int n_choices, const int win_y, const int win_x, string header)
{
    static const int buffer_y(6);
    static const int buffer_x(6);
    int i;
    int x(2);
    int y(2);

    clear();
    wclear(win);
    menu_header(win, header, win_x);

    // determine length of longest menu option
    unsigned int max_len = strlen(choices[0]);
    for(i=1; i < n_choices; ++i) {
        if (strlen(choices[i]) > max_len) {
            max_len = strlen(choices[i]);
        }
    }

    // find top-left corner of menu box
    int r_off((win_y - (n_choices + buffer_y)) / 2);
    int c_off((win_x - (max_len + buffer_x)) / 2);
    r_off = r_off > -1 ? r_off : 0;
    c_off = c_off > -1 ? c_off : 0;

    // print menu box
    wattron(win, A_REVERSE);
    for(i=0; i < (int)max_len + buffer_x; ++i) {
        mvwprintw(win, r_off, i + c_off, " ");
        mvwprintw(win, r_off + n_choices + buffer_y - 1, i + c_off, " ");
    }
    for(i=1; i < n_choices + buffer_y ; ++i) {
        mvwprintw(win, r_off + i, c_off, " ");
        mvwprintw(win, r_off + i, c_off + buffer_x + max_len - 1, " ");
    }
    wattroff(win, A_REVERSE);

    // print menu options
    for(i = 0; i < n_choices; ++i)
    {
        // Highlight the present choice
        if (highlight == i + 1) {
            wattron(win, A_REVERSE);
            mvwprintw(win, y + r_off, x + c_off, "%s", choices[i]);
            wattroff(win, A_REVERSE);
        }
        else
            mvwprintw(win, y + r_off, x + c_off, "%s", choices[i]);
        ++y;
    }

    refresh();
    wrefresh(win);
}

