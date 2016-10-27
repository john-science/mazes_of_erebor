# Makefile for the Mazes of Erebor

CC = gcc
FLAGS = -std=c++11 -O2
#FLAGS = -std=c++0x -O2
FLAGS_INC = -IEreborLib
FLAGS_EXE = -IEreborLib -lncurses -lstdc++ -L. -lereborlib -lm
LIB_DIR = EreborLib/
GUI_DIR = EreborNcursesUI/
FILES_LIB = player.o maze.o data.o story.o game.o
FILES_UI = splash.o menu.o windows.o maze_window.o
EXE_PATH = erebor


erebor: build clean_post

.PHONY: build
build: build_static_lib build_ui build_exe

.PHONY: build_static_lib
build_static_lib: $(FILES_LIB) libereborlib.a

.PHONY: build_ui
build_ui: $(FILES_UI)

.PHONY: build_exe
build_exe:
	gcc -o $(EXE_PATH) $(GUI_DIR)/main.cpp $(FILES_UI) $(FLAGS) $(FLAGS_EXE)


player.o: $(LIB_DIR)player.cpp $(LIB_DIR)player.h
	$(CC) $(FLAGS) -c $(LIB_DIR)player.cpp

maze.o: $(LIB_DIR)maze.cpp $(LIB_DIR)maze.h
	$(CC) $(FLAGS) -c $(LIB_DIR)maze.cpp

data.o: $(LIB_DIR)data.cpp $(LIB_DIR)data.h
	$(CC) $(FLAGS) -c $(LIB_DIR)data.cpp

story.o: $(LIB_DIR)story.cpp $(LIB_DIR)story.h
	$(CC) $(FLAGS) -c $(LIB_DIR)story.cpp

game.o: $(LIB_DIR)game.cpp $(LIB_DIR)game.h
	$(CC) $(FLAGS) -c $(LIB_DIR)game.cpp

libereborlib.a:
	ar rcs libereborlib.a player.o maze.o data.o story.o game.o

splash.o: $(GUI_DIR)splash.cpp $(GUI_DIR)splash.h
	$(CC) $(FLAGS) -c $(GUI_DIR)splash.cpp $(FLAGS_INC)

menu.o: $(GUI_DIR)menu.cpp $(GUI_DIR)menu.h
	$(CC) $(FLAGS) -c $(GUI_DIR)menu.cpp $(FLAGS_INC)

windows.o: $(GUI_DIR)windows.cpp $(GUI_DIR)windows.h
	$(CC) $(FLAGS) -c $(GUI_DIR)windows.cpp $(FLAGS_INC)

maze_window.o: $(GUI_DIR)maze_window.cpp $(GUI_DIR)maze_window.h
	$(CC) $(FLAGS) -c $(GUI_DIR)maze_window.cpp $(FLAGS_INC)


.PHONY: clean
clean:
	rm -f *.o *.a */*.o */*.a *.gch */*.gch erebor

.PHONY: clean_post
clean_post:
	rm -f *.o *.a */*.o */*.a *.gch */*.gch

.PHONY: rebuild
rebuild: clean build clean_post
