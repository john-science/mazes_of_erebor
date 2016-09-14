# Makefile for the Mazes of Erebor

CC = g++
FLAGS = -std=c++11 -lncurses
#FLAGS = -std=c++0x -lncurses
RELEASE_FLAGS = -O2
DEBUG_FLAGS = -O0 -g
CODE_DIR = mazes_of_erebor/
FILES = $(CODE_DIR)main.cpp $(CODE_DIR)maze.cpp $(CODE_DIR)menu.cpp $(CODE_DIR)game.cpp $(CODE_DIR)windows.cpp
OUT_EXE = erebor


erebor: build

.PHONY: build
build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FLAGS) $(FILES)

.PHONY: debug
debug: $(FILES)
	$(CC) -o $(OUT_EXE) $(DEBUG_FLAGS) $(FILES)

.PHONY: release
release: $(FILES)
	$(CC) -o $(OUT_EXE) $(RELEASE_FLAGS) $(FILES)

.PHONY: clean
clean:
	rm -f -o $(OUT_EXE) $(CODE_DIR)*.o *.o

rebuild: clean build
