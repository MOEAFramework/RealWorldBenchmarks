CC = gcc
SRC_DIR = src
C_FLAGS = -Wall -O3 -fPIC

all:
	$(CC) $(C_FLAGS) -o bin/moeaframework.o -c $(SRC_DIR)/moeaframework.c
	ld -G bin/moeaframework.o -o bin/libmoeaframework.so
	rm bin/moeaframework.o
	