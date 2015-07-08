all: system
CC=g++
CFLAGS=-std=c++11 -O2 -Wall

system: binomial_heap.h system.cpp
	$(CC) $(CFLAGS) system.cpp -o system
clean:
	rm system -f