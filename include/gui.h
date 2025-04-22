#pragma once

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define COLOR_CYCLE 3
#define DELAY 50000

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
    void uninitialize_ncurses();
};
