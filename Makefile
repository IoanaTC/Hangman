# Makefile for vulnerable hangman game

CXX := g++
CXXFLAGS := -Iinclude
LOAD_LIBS := -lncurses

SRC := src/main.cpp src/gui.cpp src/debug.cpp
OBJ := $(SRC:.cpp=.o)
TARGET := bin/hangman

DEBUG_FLAGS := -DDEBUG_MODE -g
RELEASE_FLAGS := -O2

ifeq ($(BUILD), debug)
	CXXFLAGS += $(DEBUG_FLAGS)
else
	CXXFLAGS += $(RELEASE_FLAGS)
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LOAD_LIBS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
