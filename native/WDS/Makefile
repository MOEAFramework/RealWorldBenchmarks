CC = gcc
SRC_DIR = src
SHARED_DIR = ../shared
SOURCE = $(SRC_DIR)/main.c $(SRC_DIR)/models.c $(SHARED_DIR)/moeaframework.c
CFLAGS = -I$(SRC_DIR)/epanet2 -I$(SHARED_DIR) -Lbin -lm -lepanet2 -Wl,-R,\.
LIBS =

ifeq ($(OS),Windows_NT)
	CFLAGS += -static
	LIBS += -lwsock32 -lWs2_32
endif

all:
	$(MAKE) -C $(SRC_DIR)/epanet2
	$(CC) -o bin/TRN -DTRN $(SOURCE) $(CFLAGS) $(LIBS)
	$(CC) -o bin/TLN -DTLN $(SOURCE) $(CFLAGS) $(LIBS)
	$(CC) -o bin/BAK -DBAK $(SOURCE) $(CFLAGS) $(LIBS)
	$(CC) -o bin/NYT -DNYT $(SOURCE) $(CFLAGS) $(LIBS)
	$(CC) -o bin/BLA -DBLA $(SOURCE) $(CFLAGS) $(LIBS)
	$(CC) -o bin/HAN -DHAN $(SOURCE) $(CFLAGS) $(LIBS)
	$(CC) -o bin/GOY -DGOY $(SOURCE) $(CFLAGS) $(LIBS)
	$(CC) -o bin/FOS -DFOS $(SOURCE) $(CFLAGS) $(LIBS)
	$(CC) -o bin/PES -DPES $(SOURCE) $(CFLAGS) $(LIBS)
	$(CC) -o bin/MOD -DMOD $(SOURCE) $(CFLAGS) $(LIBS)
	$(CC) -o bin/BIN -DBIN $(SOURCE) $(CFLAGS) $(LIBS)
	$(CC) -o bin/EXN -DEXN $(SOURCE) $(CFLAGS) $(LIBS)
	