
#include <ncurses.h>
#include "maze.h"
#include "menu.h"


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
    gen_entrances_opposites(player, finish, nrows, ncols);
    maze_print(menu_win, maze, MAX_SIZE, nrows, ncols, player, finish);

    while (true) {
        // input and update
        //c = wgetch(menu_win);
        c = getch();
        switch (c) {
            case 10:  // enter
                // increase the size of the maze (to a limit)
                if (ncols < MAX_SIZE) {
                    ncols += 2;
                }
                if (nrows < (MAX_SIZE / 2)) {
                    nrows += 2;
                }
                // generate a new maze
                backtracking_maze_gen(maze, MAX_SIZE, nrows, ncols);
                gen_entrances_opposites(player, finish, nrows, ncols);
                break;
            case 113:  // q
                return menu_main;
            default:
                break;
        }

        maze_print(menu_win, maze, MAX_SIZE, nrows, ncols, player, finish);
    }

    clear();
}

