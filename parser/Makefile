CC = gcc
CFLAGS = -g -std=c99 -Wall -Werror

SRC := $(shell find src -name '*.c')

sets: $(SRC)
	mkdir -p bin
	$(CC) -o bin/$@ $(CFLAGS) $^ -DMAIN

test: $(SRC)
	mkdir -p bin
	$(CC) -o bin/$@ $(CFLAGS) $^ -DTEST

.PHONY: clean
clean:
	rm -rf bin/
	rm vgcore*
