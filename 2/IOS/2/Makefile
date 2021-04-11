
CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pthread -pedantic -g
.PHONY: clean all pack

all: proj2.o
	$(CC) $(CFLAGS) -o proj2 proj2.o

clean:
	rm proj2.o proj2

pack:
	zip proj2.zip proj2.h proj2.c Makefile

proj2.o: proj2.h