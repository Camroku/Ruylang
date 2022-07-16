CC=gcc
CFLAGS=-O2 -Wall -Wextra -fno-exceptions -I./include --std=c99 -g -Wno-comment
SOURCES=ruy.c $(wildcard components/*.c)

build:
	$(CC) $(SOURCES) $(CFLAGS) -o ruy

clean:
	rm -f *.o ruy
