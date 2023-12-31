CC = gcc
SHARED_DIR = ../shared
C_FLAGS = -Wall -O3 -fPIC

ifeq ($(OS),Windows_NT)
	LIB_NAME = moeaframework.dll
else
	LIB_NAME = libmoeaframework.so
endif

all:
	$(CC) $(C_FLAGS) -shared -o bin/$(LIB_NAME) $(SHARED_DIR)/moeaframework.c
	