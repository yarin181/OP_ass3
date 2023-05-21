CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -pthread

ex3.out: ex3.c ex3.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o ex3.out ex3.c

.PHONY: clean

clean:
	rm -f ex3.out
