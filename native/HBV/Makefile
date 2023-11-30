# Copyright (C) 2011-2012 Josh Kollat, Jon Herman, Patrick Reed and others.

# The HBV Benchmark Problem is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# The HBV Benchmark Problem is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public License
# along with the HBV Benchmark Problem.  If not, see <http://www.gnu.org/licenses/>.

#*************************************************************
#* Makefile
#*
#* Purpose:
#*   Build HBV independently of e-NSGAII
#*
#* Version:
#*   May 2011 - Jon Herman. Most stolen from Josh's eNSGAII Makefile.
#*   Sept 2015 - David Hadka. Simplifies makefile.
#**************************************************************

TARGET = hbv
SRC_DIR = src
CC_SERIAL = g++

#If profiling, add the -pg flag in the last line of these as well
C_FLAGS = -O3 -DOUT_TO_SCREEN -DHBVMOD -DRANGER_MEMORY_TRICK -DSERIAL -lm
C_FLAGS_DEBUG = -g -O0 -DOUT_TO_SCREEN -DHBVMOD -DRANGER_MEMORY_TRICK -DSERIAL -lm

ifeq ($(OS),Windows_NT)
    C_FLAGS += -static-libgcc -static-libstdc++
    C_FLAGS_DEBUG += -static-libgcc -static-libstdc++
else
    C_FLAGS += -DLINUX
    C_FLAGS_DEBUG += -DLINUX
endif

HBV_SOURCE = $(SRC_DIR)/HBVMain.cpp \
    $(SRC_DIR)/HBV.cpp \
    $(SRC_DIR)/HBVInput.cpp \
    $(SRC_DIR)/HamonEvap.cpp \
    $(SRC_DIR)/MOPEXData.cpp \
    $(SRC_DIR)/QuickSort.cpp \
    $(SRC_DIR)/WaterObjectives.cpp

all:
	$(CC_SERIAL) -o bin/$(TARGET) $(C_FLAGS) $(HBV_SOURCE) 
	
debug:
	$(CC_SERIAL) -o bin/$(TARGET) $(C_FLAGS_DEBUG) $(HBV_SOURCE)

