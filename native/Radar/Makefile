CC = gcc
SHARED_DIR = ../shared
CFLAGS = -Wall -O3 -fPIC -std=c11
LIBS =

ifeq ($(OS),Windows_NT)
	TARGET = moeaframework.dll
	LIBS += -lwsock32 -lWs2_32
else
	TARGET = libmoeaframework.so
endif

all:
	$(CC) $(CFLAGS) -shared -o bin/$(TARGET) $(SHARED_DIR)/moeaframework.c $(LIBS)
	