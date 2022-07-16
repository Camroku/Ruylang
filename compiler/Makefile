CC=gcc
CFLAGS=-O2 -Wall -Wextra -fno-exceptions -I./include --std=c99 -fms-extensions
SOURCES=ruy.c components/*.c

build:
	$(CC) $(SOURCES) $(CFLAGS) -o ruy

clean:
	rm -f *.o ruy
