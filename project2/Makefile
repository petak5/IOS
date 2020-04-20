CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic

proj2: proj2.o
	$(CC) $(CFLAGS) $^ -o $@

proj2.o: proj2.c
	$(CC) $(CFLAGS) -c $< -o $@
