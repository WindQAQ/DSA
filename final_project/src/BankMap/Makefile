all: final_project
CC=g++
CFLAG=-std=c++11 -O2

final_project: md5.cpp Account.cpp final_main.cpp
	$(CC) $(CFLAG) md5.cpp Account.cpp final_main.cpp -o final_project

clean:
	rm final_project -rf