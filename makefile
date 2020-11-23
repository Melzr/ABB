CC=gcc
CFLAGS=-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
CVAL=--leak-check=full --track-origins=yes --show-reachable=yes

val2: abb2
	valgrind $(CVAL) ./abb2

abb2: abb.h abb.c pruebas.c
	$(CC) abb.c pruebas.c -o abb2 $(CFLAGS)

val: abb
	valgrind $(CVAL) ./abb

abb: abb.h abb.c abb_minipruebas.c
	$(CC) abb.c abb_minipruebas.c -o abb $(CFLAGS)

ejecutar: abb
	./abb

.PHONY: clean
clean:
	rm abb