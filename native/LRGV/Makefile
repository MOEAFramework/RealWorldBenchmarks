CC=g++
TARGET=lrgv
SRC_DIR = src
C_FLAGS=-O3 -I$(SRC_DIR) -DLINUX
SOURCE = $(SRC_DIR)/lrgv_main.cpp \
	$(SRC_DIR)/moeaframework.c \
	$(SRC_DIR)/lrgv_functions.cpp \
	$(SRC_DIR)/lrgv_input.cpp \
	$(SRC_DIR)/lrgv_memory.cpp \
	$(SRC_DIR)/lrgv_rand.cpp \
	$(SRC_DIR)/lrgv_roulette.cpp \
	$(SRC_DIR)/lrgv_Simulation.cpp \
	$(SRC_DIR)/lrgv_utility.cpp \
	$(SRC_DIR)/rand.cpp

all:
	$(CC) $(C_FLAGS) -o bin/$(TARGET) $(SOURCE)

