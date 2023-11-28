CC = g++
SRC_DIR = src
SHARED_DIR = ../shared
TARGET = lake
C_FLAGS = -O3 -Wno-unused-local-typedefs -I$(SRC_DIR) -I$(SHARED_DIR) -I$(SRC_DIR)/boost_1_56_0 
SOURCE = $(SRC_DIR)/main-lake.cpp $(SHARED_DIR)/moeaframework.c

all:
	mkdir -p bin
	$(CC) $(C_FLAGS) -o bin/$(TARGET) $(SOURCE) 

