
// global variables
const int WINDOW_WIDTH(80);
const int WINDOW_HEIGHT(22);
const int MENU_WIDTH(30);
const int MENU_HEIGHT(10);
const int GAME_WIDTH(73);  // TODO: Combine with master window dimensions?
const int GAME_HEIGHT(1 + GAME_WIDTH / 2);

// forward declaration
WINDOW* init_window();
void init_menu_window(WINDOW *win);
void init_maze_window(WINDOW *win);
