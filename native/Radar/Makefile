CC = gcc
SHARED_DIR = ../shared
C_FLAGS = -Wall -O3 -fPIC

all:
	$(CC) $(C_FLAGS) -o bin/moeaframework.o -c $(SHARED_DIR)/moeaframework.c
	ld -G bin/moeaframework.o -o bin/libmoeaframework.so
	rm bin/moeaframework.o
	