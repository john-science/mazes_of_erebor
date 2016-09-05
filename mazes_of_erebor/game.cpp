
#include <ncurses.h>
#include "maze.h"
#include "menu.h"

// forward declarations
void success_splash();
game_state easy_ui(WINDOW *menu_win);
//game_state medium_ui(WINDOW *menu_win);
//game_state hard_ui(WINDOW *menu_win);


game_state easy_ui(WINDOW *menu_win)
{
    const int MAX_SIZE = 71;
    bool maze[MAX_SIZE * MAX_SIZE];
    int nrows = 19;
    int ncols = 31;
    int player[2] = {1, 1};
    int finish[2] = {1, 1};
    int c;

    // generate a new maze
    backtracking_maze_gen(maze, MAX_SIZE, nrows, ncols);
    gen_entrances_opposites(maze, MAX_SIZE, player, finish, nrows, ncols);
    maze_print_easy(menu_win, maze, MAX_SIZE, nrows, ncols, player, finish);

    while (true) {
        // input and update
        c = getch();
        switch (c) {
            case KEY_UP:
                if (maze_valid_move(maze, MAX_SIZE, nrows, ncols, player[0] - 1, player[1])) {
                    player[0] -= 1;
                }
                break;
            case KEY_DOWN:
                if (maze_valid_move(maze, MAX_SIZE, nrows, ncols, player[0] + 1, player[1])) {
                    player[0] += 1;
                }
                break;
            case KEY_LEFT:
                if (maze_valid_move(maze, MAX_SIZE, nrows, ncols, player[0], player[1] - 1)) {
                    player[1] -= 1;
                }
                break;
            case KEY_RIGHT:
                if (maze_valid_move(maze, MAX_SIZE, nrows, ncols, player[0], player[1] + 1)) {
                    player[1] += 1;
                }
                break;
            case 113:  // q
                return menu_main;
            default:
                break;
        }

        // If you reach the end, start over in a new maze
        if (player[0] == finish[0] && player[1] == finish[1]) {
            success_splash();

            // increase the size of the maze (to a limit)
            if (ncols < MAX_SIZE) {
                ncols += 2;
            }
            if (nrows < (MAX_SIZE / 2)) {
                nrows += 2;
            }
            // generate a new maze
            backtracking_maze_gen(maze, MAX_SIZE, nrows, ncols);
            gen_entrances_opposites(maze, MAX_SIZE, player, finish, nrows, ncols);
        }

        maze_print_easy(menu_win, maze, MAX_SIZE, nrows, ncols, player, finish);
    }

    clear();
}


/**
 *  A quick splash screen to congratulate the player on finishing the maze.
 *  NOTE: This is partially just a place-holder for a better splash screen.
 */
void success_splash() {
    clear();
    mvprintw(1, 1, "Success! You found your way through the maze.");
    mvprintw(3, 1, "You delver deeper into Erebor...");
    refresh();

    getch();
}
