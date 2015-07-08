all: tree forest
CC = g++
CFLAGS = -O2 -std=c++11 -Wall

tree: tree.cpp
	$(CC) $(CFLAGS) -o tree tree.cpp
forest: forest.cpp
	$(CC) $(CFLAGS) -o forest forest.cpp

clean:
	rm -f tree forest