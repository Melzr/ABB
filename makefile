CC=gcc
CFLAGS=-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
CVAL=--leak-check=full --track-origins=yes --show-reachable=yes

probar: abb
	valgrind $(CVAL) ./abb

abb: abb.h abb.c abb_minipruebas.c
	$(CC) abb.c abb_minipruebas.c -o abb $(CFLAGS)

ejecutar: abb
	./abb

.PHONY: clean
clean:
	rm abb