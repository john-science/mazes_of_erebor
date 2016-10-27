

#include <ncurses.h>
#include <string>
#include <vector>
#include "windows.h"
#include "story.h"
#include "player.h"


using namespace std;

// forward declarations
vector<string> format_text(const string txt, unsigned int num_cols);
void content_screen(WINDOW *win, string txt);
void intro_splash(WINDOW *win);
void success_splash(WINDOW *win, const int count);


/**
 *    Splash screen, for the start of the game
 */
void intro_splash(WINDOW *win) {
    content_screen(win, gen_intro_text());
}


/**
 *    Splash screen, after you finish a maze
 */
void success_splash(WINDOW *win, const int count) {
    content_screen(win, build_success_text(count));
    full_box_clear(win);
}


/**
 *    Default function to display text content
 */
void content_screen(WINDOW *win, string txt) {
    int win_y(15);
    int win_x(15);
    mvwin(win, 0, 0);
    getmaxyx(stdscr, win_y, win_x);
    wresize(win, win_y, win_x);
    wclear(win);
    box(win, 0, 0);

    if (win_y < 6 || win_x < 12) {return;}

    unsigned int num_rows(win_y - 2);
    int num_cols(win_x - 4);

    vector<string> lines(format_text(txt, num_cols));
    unsigned int row(win_y / 2 > 1 ? win_y / 2 : 1);

    for (int i=0; i < (int)lines.size(); ++i) {
        mvprintw(row, 2, lines[i].c_str());
        row += 1;
        if (row == num_rows) {
            row = 1;
            wrefresh(win);
            getch();
            wclear(win);
            box(win, 0, 0);
        }
    }

    wrefresh(win);
    getch();
}


/**
 *    Format potentially long text for print-out.
 */
vector<string> format_text(const string txt, unsigned int num_cols) {
    vector<string> lines;
    unsigned int i(0);
    unsigned int last_space(0);
    unsigned int last_end(0);

    // if the line is short, skip all this work
    if (txt.length() < num_cols) {
        lines.push_back(txt);
        return lines;
    }

    // loop through each character
    while (i < txt.length()) {
        // note the spaces, for clean line breaks
        if (txt[i] == ' '){
            last_space = i;
        }

        // break on EOL and when you are past the column count
        if (txt[i] == '\n') {
            lines.push_back(txt.substr(last_end, i - last_end));
            last_end = i + 1;
        } else if ((i - last_end) == num_cols) {
            if (last_end >= last_space) {
                lines.push_back(txt.substr(last_end, num_cols));
                last_end += num_cols;
            } else {
                lines.push_back(txt.substr(last_end, last_space - last_end));
                last_end = last_space + 1;
            }
        }
        i += 1;
    }

    // add last part of string
    lines.push_back(txt.substr(last_end));
    return lines;
}
