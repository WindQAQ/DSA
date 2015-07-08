all: final_project
CC=g++
CFLAG=-std=c++11 -O3

final_project: md5.o rb.o Account.o create.o Bankrb.cpp
	$(CC) $(CFLAG) md5.o Account.o rb.o create.o Bankrb.cpp -o final_project

md5.o: md5.h md5.cpp
	$(CC) $(CFLAG) md5.cpp -c

rb.o: rb.h rb.c
	gcc -O3 rb.c -c

Account.o: Account.h Account.cpp
	$(CC) $(CFLAG) Account.cpp -c

create.o: rb.o Account.o create.cpp
	$(CC) $(CFLAG) create.cpp -c

clean:
	rm *.o final_project -rf