CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2
CFLAGS_VALGRIND = --leak-check=full --track-origins=yes --show-reachable=yes --show-leak-kinds=all

compilar:
	$(CC) main.c src/*.c src/*.h $(CFLAGS) -o main

valgrind: compilar
	valgrind $(CFLAGS_VALGRIND) ./main 2>&1