
#include <ncurses.h>

// TODO: Should these be in the header, so everyone can play with them?
//       Or should they be in main, or in a window class?
#define MENU_WIDTH 30
#define MENU_HEIGHT 10
#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 22

/**
 * TODO: build continuable menus
char const *choices_cont[] = {"Start New", "Continue", "Quit",};
int n_choices_cont = sizeof(choices_cont) / sizeof(char *);
*/


enum game_state {menu_main, menu_diff,
                 game_easy, game_medium, game_hard, game_intro, game_splash,
                 quit};


void print_menu(WINDOW *menu_win, int highlight, char const **choices, int n_choices);
game_state main_menu(WINDOW *menu_win);
game_state diff_menu(WINDOW *menu_win);


/**
 *  Menu for: Game Difficulty selection
 */
game_state diff_menu(WINDOW *menu_win)
{
    char const *choices[] = {"Easy", "Medium", "Hard",};
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight(1);
    int choice(-999);
    int startx(0);
    int starty(0);
    int c;

    startx = (WINDOW_WIDTH - MENU_WIDTH) / 2;
    starty = (WINDOW_HEIGHT - MENU_HEIGHT) / 2;

    menu_win = newwin(MENU_HEIGHT, MENU_WIDTH, starty, startx);
    keypad(menu_win, TRUE);

    clear();
    mvprintw(3, WINDOW_WIDTH / 2 - 17, "May you live in interesting times.");
    refresh();

    print_menu(menu_win, highlight, choices, n_choices);
    while (true) {
        // input and update
        c = wgetch(menu_win);
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
            default:
                break;
        }
        // display
        print_menu(menu_win, highlight, choices, n_choices);
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
game_state main_menu(WINDOW *menu_win)
{
    char const *choices[] = {"Start New", "Quit",};
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight(1);
    int choice(-999);
    int startx(0);
    int starty(0);
    int c;

    startx = (WINDOW_WIDTH - MENU_WIDTH) / 2;
    starty = (WINDOW_HEIGHT - MENU_HEIGHT) / 2;

    menu_win = newwin(MENU_HEIGHT, MENU_WIDTH, starty, startx);  // TODO: Move elsewhere?
    keypad(menu_win, TRUE);

    clear();
    mvprintw(1, WINDOW_WIDTH / 2 - 10, "The Halls of Erebor");
    refresh();

    print_menu(menu_win, highlight, choices, n_choices);
    while (true) {
        c = wgetch(menu_win);
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
            default:
                mvprintw(WINDOW_HEIGHT, 0, "Charcter pressed was %3d Hopefully it can be printed as '%c' ", c, c);
                refresh();
                break;
        }
        print_menu(menu_win, highlight, choices, n_choices);
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
void print_menu(WINDOW *menu_win, int highlight, char const **choices, int n_choices)
{
    int i;
    int x(2);
    int y(2);

    box(menu_win, 0, 0);
    for(i = 0; i < n_choices; ++i)
    {
        // Highlight the present choice
        if (highlight == i + 1) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        ++y;
    }
    wrefresh(menu_win);
}

