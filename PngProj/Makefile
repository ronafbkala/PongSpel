OBJS = main.c application.c ball.c paddle.c counter.c
OBJ_NAME = out
CC = gcc


INCLUDE_PATHS = -IC:\mingw_dev_lib\include\SDL2
LIBRARY_PATHS = -LC:\mingw_dev_lib\lib
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_net -lSDL2_ttf

all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)

