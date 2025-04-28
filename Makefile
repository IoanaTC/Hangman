# Makefile for vulnerable hangman game

CXX := g++
CXXFLAGS := -Iinclude -Wl,-rpath,'$$ORIGIN'
LOAD_LIBS := -lncurses

SRC := src/debug.cpp src/game_session.cpp src/gui.cpp src/main.cpp
OBJ := $(SRC:.cpp=.o)
TARGET := hangman
PLUGIN := winning_sound.so

WIN_SOUND_FILE := win_sound.wav

DEBUG_FLAGS := -DDEBUG_MODE -g
RELEASE_FLAGS := -O2

ifeq ($(BUILD), debug)
	CXXFLAGS += $(DEBUG_FLAGS)
else
	CXXFLAGS += $(RELEASE_FLAGS)
endif

all: $(PLUGIN) $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LOAD_LIBS)

$(PLUGIN): winning_sound.cpp
	g++ -shared -fPIC -o $@ $< -lsndfile -lao

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(PLUGIN)

.PHONY: all clean copy_win_sound
