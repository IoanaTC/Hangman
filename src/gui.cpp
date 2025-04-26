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
   cbreak();
   noecho();
   curs_set(FALSE);
   start_color();

   for(unsigned int i = 1; i <= COLOR_CYCLE; i++) {
       init_pair(i, i + 5, COLOR_BLACK);
   }
}
void GraphicalInterface::start_interface()
{
    unsigned int color_index = 1;

    for(unsigned int i = 0; i < COLOR_CYCLE; i++) {
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
        color_index = color_index % COLOR_CYCLE + 1;

        usleep(DELAY);
    }
}
void GraphicalInterface::show_presentation_screen() {
    clear();

    mvprintw(_center_y - 1, _center_x - 15, "Ultimate Hangman game. Welcome!");
    mvprintw(_center_y, _center_x - 16, "Disclaimer: You break it, you pay!");
    mvprintw(_center_y + 1, _center_x - 12, "Press any key to start...");

    refresh();
    //keypad(stdscr, true);
    noecho();
    getch();

    //int character;
    //do {
    //    character = (int) getch();
    //} while(character < 32 || character > 126);

    //keypad(stdscr, false);
    echo();
}
void GraphicalInterface::uninitialize_ncurses() {
    endwin();
}
/*
void GraphicalInterface::show_round_interface(const char * current_word, unsigned int word_length, 
        unsigned int number_of_mistakes)
{
    clear();

    int word_width = word_length * 2 - 1;
    int start_x = _center_x - word_width / 2;
    int end_x = _center_x + word_width / 2;

    for (int x = start_x, letter = 0; x < end_x && letter < word_length; x += 1, letter += 1) {
        if(current_word[letter]) {
            mvaddch(_center_y, x, current_word[letter]);
        } else {
            mvaddch(_center_y, x, '_');
        }
    }
    refresh();

}
*/
void GraphicalInterface::show_round_interface(const char * current_word, unsigned int word_length, unsigned int number_of_mistakes)
{
    clear();

    int word_width = word_length * 2 - 1; // each letter and a space between
    int start_x = _center_x - word_width / 2;
    int word_y = _center_y;

    for (unsigned int letter = 0; letter < word_length; letter++) {
        int x = start_x + letter * 2;

        mvaddch(word_y + 1, x, '_');

        if ((int)current_word[letter] != 0) {
            mvaddch(word_y, x, current_word[letter]);
        }
    }
    draw_hangman(number_of_mistakes);

    refresh();
}
void GraphicalInterface::draw_hangman(unsigned int mistakes)
{
    int base_x = _max_x - 20;
    int base_y = 2;

    mvprintw(base_y, base_x, "+---+");
    mvprintw(base_y + 1, base_x, "|   |");

    if (mistakes >= 1)
        mvprintw(base_y + 2, base_x + 2, "O");

    if (mistakes >= 2)
        mvprintw(base_y + 3, base_x + 2, "|");

    if (mistakes >= 3)
        mvprintw(base_y + 3, base_x + 1, "/");

    if (mistakes >= 4)
        mvprintw(base_y + 3, base_x + 3, "\\");

    if (mistakes >= 5)
        mvprintw(base_y + 4, base_x + 1, "/");

    if (mistakes >= 6)
        mvprintw(base_y + 4, base_x + 3, "\\");

    if (mistakes >= 7) {
        mvprintw(base_y + 2, base_x + 2, "X");
    }
}
