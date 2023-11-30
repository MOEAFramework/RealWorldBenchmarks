CC=g++
TARGET=lrgv
SRC_DIR = src
SHARED_DIR = ../shared
C_FLAGS=-O3 -I$(SRC_DIR) -I$(SHARED_DIR)

ifeq ($(OS),Windows_NT)
	C_FLAGS += -static-libgcc -static-libstdc++
else
    C_FLAGS += -DLINUX
endif

SOURCE = $(SRC_DIR)/lrgv_main.cpp \
	$(SHARED_DIR)/moeaframework.c \
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

