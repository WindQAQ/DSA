all: final_project
CC=g++
CFLAG=-std=c++11 -O3

final_project: md5.o avl.o Account.o create.o BankAvl.cpp
	$(CC) $(CFLAG) md5.o Account.o avl.o create.o BankAvl.cpp -o final_project

md5.o: md5.h md5.cpp
	$(CC) $(CFLAG) md5.cpp -c

avl.o: avl.h avl.c
	gcc -O3 avl.c -c

Account.o: Account.h Account.cpp
	$(CC) $(CFLAG) Account.cpp -c

create.o: avl.o Account.o create.cpp
	$(CC) $(CFLAG) create.cpp -c

clean:
	rm *.o final_project -rf