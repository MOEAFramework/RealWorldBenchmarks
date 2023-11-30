CC = gcc
SHARED_DIR = ../shared
C_FLAGS = -Wall -O3 -fPIC

PLATFORM := $(shell uname)
ifeq  ($(PLATFORM),Linux)
EXT = so
else
EXT = dll
endif

all:
	$(CC) $(C_FLAGS) -shared -o bin/moeaframework.$(EXT) -c $(SHARED_DIR)/moeaframework.c
	
