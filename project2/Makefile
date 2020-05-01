CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread

PROJ=proj2

.PHONY: clean zip

$(PROJ): $(PROJ).o $(PROJ).h
	$(CC) $(CFLAGS) $^ -o $@

$(PROJ).o: $(PROJ).c $(PROJ).h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.out $(PROJ)

zip: $(PROJ).c $(PROJ).h
	zip proj2.zip *.c *.h Makefile
