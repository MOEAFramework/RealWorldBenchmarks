CC = g++
SRC_DIR = src
SHARED_DIR = ../shared
TARGET = lake
CFLAGS = -O3 -Wno-unused-local-typedefs -I$(SRC_DIR) -I$(SHARED_DIR) -I$(SRC_DIR)/boost_1_56_0
SOURCE = $(SRC_DIR)/main-lake.cpp $(SHARED_DIR)/moeaframework.c
LIBS =

ifeq ($(OS),Windows_NT)
	CFLAGS += -static
	LIBS += -lwsock32 -lWs2_32
endif

all:
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$(TARGET) $(SOURCE) $(LIBS)

