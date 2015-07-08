all: 3_4_1.out 3_4_2.out
CC = g++
CFLAGS = -O2 -std=c++11 -Wall

3_4_1.out: 3_4_1.cpp
	$(CC) $(CFLAGS) -o 3_4_1.out 3_4_1.cpp
3_4_2.out: 3_4_2.cpp
	$(CC) $(CFLAGS) -o 3_4_2.out 3_4_2.cpp

clean:
	rm -f 3_4_1.out 3_4_2.out

