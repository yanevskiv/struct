TARGET  = main
CC      = gcc
LIBS    = -lstruct
CFLAGS  = -Llib -Iinclude 

main: main.c
	gcc -Iinc main.c -o $@

clean:
	rm -f ./main
	rm -f ./a.out

.PHONY: clean
