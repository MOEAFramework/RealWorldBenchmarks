CC = gcc
SHARED_DIR = ../shared
C_FLAGS = -Wall -O3 -fPIC

ifeq ($(OS),Windows_NT)
	EXT = dll
else
	EXT = so
endif

all:
	$(CC) $(C_FLAGS) -shared -o bin/moeaframework.$(EXT) -c $(SHARED_DIR)/moeaframework.c
	
