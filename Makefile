IDIR = ./include
CC = gcc
CFLAGS = -I$(IDIR)

#OBJS specifies which files to compile as part of the project
OBJS = ./src/main.c ./src/filehandler.c ./src/graphics.c

#linker flags
LINKER_FLAGS = -lm -lSDL2 -lSDL2_ttf

#name of the executable
OBJ_NAME = program

#compile
all : $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LINKER_FLAGS) -o ./bin/$(OBJ_NAME)
