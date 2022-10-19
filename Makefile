TARGET  = main
CC      = gcc
LIBS    = -lstruct
CFLAGS  = -Llib -Iinclude 


main: main.c
	gcc -I include main.c -o $@

clean:
	rm -f ./main

.PHONY: clean
