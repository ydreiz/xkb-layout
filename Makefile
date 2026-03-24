.PHONY:

CC_LIBS = -lX11
XX_FLAGS = -Wall -Wextra -pedantic -ggdb

all: main.c
	gcc $(XX_FLAGS) -o xkb-layout main.c $(CC_LIBS)
