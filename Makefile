CC=gcc
CFLAGS=-Wall -std=c99 -D_GNU_SOURCE

all: smallsh

smallsh: smallsh.c
	$(CC) $(CFLAGS) -o smallsh smallsh.c
