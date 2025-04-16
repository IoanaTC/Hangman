#include "gui.h"

GraphicalInterface * GraphicalInterface::_gui_instance = nullptr;

GraphicalInterface::GraphicalInterface()
{
    _max_x = _max_y = _center_x = _center_y = 0;
}
GraphicalInterface * GraphicalInterface::GetInstance()
{
    if(!_gui_instance) {
        _gui_instance = new GraphicalInterface();
        if(!_gui_instance) {
            // constructor failed
            return nullptr;
        }
    }
    return _gui_instance;
}
void GraphicalInterface::get_screen_measurements()
{
    getmaxyx(stdscr, _max_y, _max_x);

    _center_x = _max_x / 2;
    _center_y = _max_y / 2;
}
void GraphicalInterface::initialize_ncurses()
{
   initscr(); 
   noecho();
   curs_set(FALSE);
   start_color();

   for(unsigned int i = 0; i <= COLOR_CYCLE; i++) {
       init_pair(i, i, COLOR_BLACK);
   }
}
void GraphicalInterface::start_interface()
{
    unsigned int color_index = 1;
    while(1) {
        clear();

        for(int x = 0; x <= _max_x; x++) {
            for(int y = 0; y <= _max_y; y++) {
                if(rand() % 10 < 2) {
                    attron(COLOR_PAIR(color_index));
                    mvprintw(y, x, "%c", (rand() % 94) + 33);
                    attroff(COLOR_PAIR(color_index));
                }
            }
        }

        refresh();
        color_index = (color_index + 1) % COLOR_CYCLE;

        usleep(DELAY);
    }
}
void GraphicalInterface::show_presentation_screen() {

}
void GraphicalInterface::uninitialize_ncurses() {
    endwin();
}
