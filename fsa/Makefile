#
# File: Makefile
# Creator: George Ferguson
# Created: Thu Jun 30 11:00:49 2016
#
# Sample Makefile distributed with CSC173 Project 1 code bundle
#
# This Makefile builds the test programs for the data structures
# distributed with the bundle, and show how you might use it to
# build YOUR program for the project.
#

CC = gcc
CFLAGS = -g -std=c99 -Wall -Werror -lm

SRC := $(shell find src -name '*.c')

main: $(SRC)
	mkdir -p bin
	$(CC) -o bin/$@.out $(CFLAGS) $^

.PHONY: clean
clean:
	rm -rf bin/
	rm vgcore*
