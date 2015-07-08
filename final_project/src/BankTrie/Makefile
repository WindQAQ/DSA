all: final_project
CC=g++
CFLAG=-std=c++11 -O2

final_project: md5.o trie.o final_project.cpp
	$(CC) $(CFLAG) md5.o trie.o final_project.cpp -o final_project

md5.o: md5.cpp md5.h
	$(CC) $(CFLAG) $< -c

trie.o: trie.cpp trie.h
	$(CC) $(CFLAG) $< -c

clean:
	rm final_project *.o -rf 