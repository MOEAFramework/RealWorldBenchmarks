CC = g++
TARGET = lrgv
SRC_DIR = src
SHARED_DIR = ../shared
CFLAGS = -O3 -I$(SRC_DIR) -I$(SHARED_DIR) -std=c++11
LIBS =

ifeq ($(OS),Windows_NT)
	CFLAGS += -static
	LIBS += -lwsock32 -lWs2_32
else
    CFLAGS += -DLINUX
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
	$(CC) $(CFLAGS) -o bin/$(TARGET) $(SOURCE) $(LIBS)

