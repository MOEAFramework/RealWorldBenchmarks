CC = g++
SRC_DIR = src
TARGET = lake
C_FLAGS = -O3 -Wno-unused-local-typedefs -I$(SRC_DIR) -I$(SRC_DIR)/boost_1_56_0 
SOURCE = $(SRC_DIR)/main-lake.cpp $(SRC_DIR)/moeaframework.cpp

all:
	mkdir -p bin
	$(CC) $(C_FLAGS) -o bin/$(TARGET) $(SOURCE) 

