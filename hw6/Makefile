CC = gcc
CXX = g++
CFLAGS = -O2 -std=c99 -Wall
CXXFLAGS = -O2 -std=c++11 -Wall
PROB1 = hw6_2
PROB2 = hw6_3
TARGET = $(PROB1) $(PROB2)
OBJ1 = avl.o rb.o bst.o hw6_2.o
OBJ2 = avl_ntudsa.o hw6_3.o
OBJS = $(OBJ1) $(OBJ2)

RM = rm -rf

-include Makefile.inc;

.PHONY: all pre cleanall clean run1 run2

all: $(TARGET)

%.o:%.c
	$(CC) $(CFLAGS) $< -c

%.o:%.cpp
	$(CXX) $(CXXFLAGS) $< -c

$(PROB1): $(OBJ1)
	$(CXX) $(CXXFLAGS) $(OBJ1) -o $@

$(PROB2): $(OBJ2)
	$(CXX) $(CXXFLAGS) $(OBJ2) -o $@

pre: $(OBJS)

cleanall:
	$(RM) $(TARGET)
	$(RM) $(OBJS)

clean:
	$(RM) $(TARGET)

run1: $(PROB1)
	./$(PROB1)

run2: $(PROB2)
	./$(PROB2)
