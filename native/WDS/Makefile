CC = gcc
SRC_DIR = src
SHARED_DIR = ../shared
SOURCE = $(SRC_DIR)/main.c $(SRC_DIR)/models.c $(SHARED_DIR)/moeaframework.c
C_FLAGS = -I$(SRC_DIR)/epanet2 -I$(SHARED_DIR) -Lbin -lm -lepanet2 -Wl,-R,\.

ifeq ($(OS),Windows_NT)
	C_FLAGS += -static
endif

all:
	$(MAKE) -C $(SRC_DIR)/epanet2
	$(CC) -o bin/TRN -DTRN $(SOURCE) $(C_FLAGS)
	$(CC) -o bin/TLN -DTLN $(SOURCE) $(C_FLAGS)
	$(CC) -o bin/BAK -DBAK $(SOURCE) $(C_FLAGS)
	$(CC) -o bin/NYT -DNYT $(SOURCE) $(C_FLAGS)
	$(CC) -o bin/BLA -DBLA $(SOURCE) $(C_FLAGS)
	$(CC) -o bin/HAN -DHAN $(SOURCE) $(C_FLAGS)
	$(CC) -o bin/GOY -DGOY $(SOURCE) $(C_FLAGS)
	$(CC) -o bin/FOS -DFOS $(SOURCE) $(C_FLAGS)
	$(CC) -o bin/PES -DPES $(SOURCE) $(C_FLAGS)
	$(CC) -o bin/MOD -DMOD $(SOURCE) $(C_FLAGS)
	$(CC) -o bin/BIN -DBIN $(SOURCE) $(C_FLAGS)
	$(CC) -o bin/EXN -DEXN $(SOURCE) $(C_FLAGS)