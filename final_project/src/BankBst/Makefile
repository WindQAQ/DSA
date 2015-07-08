all: final_project
CC=g++
CFLAG=-std=c++11 -O3

final_project: md5.o bst.o Account.o create.o BankBst.cpp
	$(CC) $(CFLAG) md5.o Account.o bst.o create.o BankBst.cpp -o final_project

md5.o: md5.h md5.cpp
	$(CC) $(CFLAG) md5.cpp -c

bst.o: bst.h bst.c
	gcc -O3 bst.c -c

Account.o: Account.h Account.cpp
	$(CC) $(CFLAG) Account.cpp -c

create.o: bst.o Account.o create.cpp
	$(CC) $(CFLAG) create.cpp -c

clean:
	rm *.o final_project -rf