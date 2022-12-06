# CC = gcc
# CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2
# CFLAGS_VALGRIND = --leak-check=full --track-origins=yes --show-reachable=yes --show-leak-kinds=all
VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2
CC = gcc

compilar:
	$(CC) main.c src/*.c src/*.h $(CFLAGS) -o main

valgrind: compilar
	valgrind $(VALGRIND_FLAGS) ./main 2>&1

valgrind-pruebas: pruebas
	valgrind $(VALGRIND_FLAGS) ./pruebas 2>&1

pruebas: src/*.c pruebas.c
	$(CC) $(CFLAGS) src/*.c pruebas.c -o pruebas 2>&1

