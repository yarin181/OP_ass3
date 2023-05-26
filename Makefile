CC = gcc
LDFLAGS = -pthread

ex3.out: ex3.c ex3.h
	$(CC) $(LDFLAGS) -o ex3.out ex3.c

.PHONY: clean

clean:
	rm -f ex3.out
