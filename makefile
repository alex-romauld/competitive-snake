TOP := $(realpath $(lastword $(MAKEFILE_LIST)))/..
#https://stackoverflow.com/questions/322936/common-gnu-makefile-directory-path

CC = g++
CFLAGS = -Wall -Wextra

OBJ_NAME = comp_snake

INCLUDE_PATHS = -I"$(TOP)/include"
LIBRARY_PATHS = -L"$(TOP)/lib"

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lws2_32


all: $(OBJ_NAME)

comp_snake: main.o game_components.o game_net.o
	$(CC) $(CFLAGS) -o $(OBJ_NAME) main.o game_components.o game_net.o $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

main.o: main.cpp game_components.h game_net.h
	$(CC) $(CLFLAGS) -c main.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

game_net.o: game_net.cpp game_net.h game_components.h
	$(CC) $(CFLAGS) -c game_net.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

game_components.o: game_components.cpp game_components.h
	$(CC) $(CLAGS) -c game_components.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)


.PHONY: all clean

clean:
	rm -f *.o $(OBJ_NAME)
