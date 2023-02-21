EXEC := example.exe
CFLAGS := -Wall -g -std=c++17
CPP := g++

INC_DIR := ../Reader/include/
SRC_DIR := .

SRC	= $(shell find $(SRC_DIR) -name '*.cpp')

all:
	$(CPP) $(CFLAGS) $(SRC) -o $(EXEC) -I$(INC_DIR)
