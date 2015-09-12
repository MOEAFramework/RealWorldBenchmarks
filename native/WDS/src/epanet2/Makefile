CC = gcc
SOURCE = epanet.c \
	hash.c \
	hydraul.c \
	inpfile.c \
	input1.c \
	input2.c \
	input3.c \
	mempool.c \
	output.c \
	quality.c \
	report.c \
	rules.c \
	smatrix.c

all:
ifeq ($(OS),Windows_NT)
	$(CC) -shared -o ../../bin/epanet2.lib $(SOURCE)
else
	$(CC) -shared -fPIC -o ../../bin/libepanet2.so $(SOURCE) -lm
endif