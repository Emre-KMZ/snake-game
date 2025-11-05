## Simple Makefile for terminal-snake

# Compiler and tools
CXX := g++

# Flags
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2
LDFLAGS :=
LDLIBS := -lncurses

# Files
SRC := main.cc SnakeEngine.cc
OBJ := $(SRC:.cc=.o)
BIN := snake

.PHONY: all build run clean

all: $(BIN)

build: all

$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

%.o: %.cc SnakeEngine.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(OBJ) $(BIN)

