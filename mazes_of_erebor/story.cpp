
#include <random>
#include <ncurses.h>
#include <string.h>
#include <vector>

using namespace std;

// forward declarations
void content_screen(WINDOW *win, string txt);
vector<string> format_text(const string txt, unsigned int num_cols);
void intro_splash(WINDOW *win);
const char* skewed_choice(const char* arr[], const int length, const int exp=3);
void success_splash(WINDOW *win, const int count);

// constants for splash screen
//const char* splash_exclaim[] = {""};  // Testing content screen bug
const char* splash_exclaim[] = {"", "Success! ", "Finally! ", "Whew! "};
const int n_splash_exclaim = sizeof(splash_exclaim) / sizeof(char *);
//const char* splash_success[] = {"You did it!"};  // Testing content screen bug
const char* splash_success[] = {"You did it!",
                                "You solved it!",
                                "You solved the maze!",
                                "You found a way out!",
                                "You are through the maze!",
                                "You found your way through the maze!",
                                "You are through!",
                                "You found the end of the laybrinth!"};
const int n_splash_success = sizeof(splash_success) / sizeof(char *);
// TODO: Add more of these
//const char* splash_story[] = {"You delve deeper."};  // Testing content screen bug
const char* splash_story[] = {"You delve deeper.",
                              "You take a short rest before taking the staircase down.",
                              "At the end of the maze you find a staircase leading down.",
                              "You find a staircase leading down and follow it.",
                              "Deeper and deeper into the Halls of the Mountain King...",
                              "Your torch flickers in a draft as you head down the stairs.",
                              "You hear the echoes of war drums far off in the distance.",
                              "You find a curving ramp leading further down into the mountain.",
                              "You find a narrow staircase leading down into the mountain.",
                              "You kick over a dusty old pile of Orcish remains that block the staircase.",
                              "How deep under the mountain do these tunnels go?",
                              "Above the stone doorway you find an engraved scene of a human archer killing a dragon.",
                              "Engraved along the walls of the spiral staircase are scenes of a dwarf being buried with a glowing gem."};
const int n_splash_story = sizeof(splash_story) / sizeof(char *);
const char* intro = "You are Khorin, son of Balin and Rogyr, and you can feel that the fourth "
                    "age of the world is drawing to a close. You hail from the the great "
                    "northern city of Annúminas, which your Dwarven ancestors helped rebuild."
                    "\n\nThere are very few Dwarves left in the world. Erebor, their last "
                    "great citadel, fell to a goblin army many centuries ago. There simply "
                    "weren't enough Dwarves left to defend the Lonely Mountain. Your father, "
                    "a great stone mason, was with the last human army to try and assault the "
                    "mountain. Their losses were terrible, and they never got passed the gate. "
                    "Instead, they sealed the gate with an avalanche, and fortified the "
                    "rubble into a mighty wall. Then they left the goblin army to starve in the "
                    "mines.\n\nIt has been 200 years since that battle and the humans have long "
                    "since forgot the threat. The young kings cannot believe that anyone could "
                    "long survive under ground. But they do not know the ways of Dwarven mining. "
                    "They do not know about black moth honey or grazing blind, pale livestock "
                    "on the deep moss that grows along underground streams. "
                    "And goblins have always preferred living underground. They "
                    "could tunnel out in a few short weeks, if they wanted.\n\nWord reached "
                    "your ear, though, that the last elven queen foretold huge armies "
                    "underneath the Lonely Mountain. Whose numbers are now nearly large enough "
                    "to assault Middle Earth. You have decided to defend the world by "
                    "venturing deep underneath Erebor. Far under the labyrinths and "
                    "catacombs there lies a weapon that the elven queen said could bring "
                    "down the mountain. It will collapse into the Earth, lava will consume "
                    "it, and every goblin in it will parish.\n\nDelving into the mountain would "
                    "mean facing the goblin hoards alone. Along with whatever fel beasts they "
                    "have bred. If you succeed, you will destroy the final home of Dwarves in "
                    "Middle Earth. And you will be deep under the mountain when it falls. But "
                    "you are half human, and cannot let the world fall without defending it.\n\n"
                    "You set off quietly months ago and started mining out the hidden entrance "
                    "your father left to the undermines. Sealing yourself in as you go. Today "
                    "you break through and enter the catacombs for the first and last time.";


/**
 *    Splash screen, for the start of the game
 */
void intro_splash(WINDOW *win) {
    content_screen(win, intro);
}


/**
 *    Splash screen, after you finish a maze
 */
void success_splash(WINDOW *win, const int count) {
    string txt(string(skewed_choice(splash_exclaim, n_splash_exclaim)) +
               string(skewed_choice(splash_success, n_splash_success)) + "\n\n" +
               string(skewed_choice(splash_story, n_splash_story)) + "\n\n\n" +
               string("You are now ") + to_string(count) + string(" levels under Erebor."));

    content_screen(win, txt);
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
    unsigned int row(1);

    for (int i=0; i < (int)lines.size(); ++i) {
        mvprintw(row, 2, lines[i].c_str());  // TODO: Should probably be a vector of const char*
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
 *    Randomly select an element from a text array,
 *    skewing the results toward the first elements.
 *    Use the "exp" parameter to control how strong the skewing is.
 */
const char* skewed_choice(const char* arr[], const int length, const int exp) {
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return arr[(int)(length * pow(r, exp))];
}
