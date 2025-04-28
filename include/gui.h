#pragma once

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;

#define GENERAL_MAX_SIZE 2048

#define COLOR_CYCLE 3
#define DELAY 90000

// singleton
class GraphicalInterface {
private:
    GraphicalInterface();
    static GraphicalInterface * _gui_instance;

    int _max_x, _max_y;
    int _center_x, _center_y;
public:
    GraphicalInterface(const GraphicalInterface &) = delete;
    void operator=(const GraphicalInterface &) = delete;

    static GraphicalInterface * GetInstance();
    
    void get_screen_measurements();
    void initialize_ncurses();
    void start_interface();
    void show_presentation_screen();
    void show_instructions_screen();
    void show_end_screen(unsigned short score,
            const char * username);
    void uninitialize_ncurses();

    void show_round_interface(const char * current_word, 
            unsigned int word_length, 
            unsigned int number_of_mistakes);

    void draw_hangman(unsigned int mistakes);

    void show_win_interface(unsigned int round_number,
            const char * word,
            unsigned short score);

    void show_fail_interface(unsigned int round_number, 
            const char * word,
            unsigned short score,
            const char * hint);
};
