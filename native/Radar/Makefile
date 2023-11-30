CC = gcc
SHARED_DIR = ../shared
C_FLAGS = -Wall -O3 -fPIC

ifeq ($(OS),Windows_NT)
<<<<<<< Updated upstream
=======
	C_FLAGS += -static
>>>>>>> Stashed changes
	EXT = dll
else
	EXT = so
endif

all:
	$(CC) $(C_FLAGS) -shared -o bin/moeaframework.$(EXT) -c $(SHARED_DIR)/moeaframework.c
	
