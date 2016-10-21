
#ifndef SPLASH_H
#define SPLASH_H

#include <string.h>
#include "player.h"

// declarations
void content_screen(WINDOW *win, std::string txt);
void intro_splash(WINDOW *win, PlayerData p);
void success_splash(WINDOW *win, const int count);

#endif
