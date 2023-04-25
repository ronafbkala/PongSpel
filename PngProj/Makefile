OBJS = main.c application.c ball.c paddle.c
OBJ_NAME = out
CC = gcc


INCLUDE_PATHS = -IC:\msys2\mingw64\include\SDL2
LIBRARY_PATHS = -LC:\msys2\mingw64\lib
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_net -lSDL2_ttf

all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)

