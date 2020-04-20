CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic

PROJ=proj2

.PHONY: clean zip

$(PROJ): $(PROJ).o $(PROJ).h
	$(CC) $(CFLAGS) $^ -o $@

$(PROJ).o: $(PROJ).c $(PROJ).h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(PROJ)

zip: $(PROJ).c $(PROJ).h
	zip xurgos00.zip *.c *.h Makefile
