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
        
        struct timespec req;
        req.tv_sec = DELAY;
        req.tv_nsec = 0;
        nanosleep(&req, NULL);
    }
}
void GraphicalInterface::show_presentation_screen() {
    clear();

    mvprintw(_center_y - 1, _center_x - 15, "Ultimate Hangman game. Welcome!");
    mvprintw(_center_y, _center_x - 16, "Disclaimer: You break it, you pay!");
    mvprintw(_center_y + 1, _center_x - 12, "Press any key to start...");

    refresh();

    noecho();
    getch();
    echo();
}
void GraphicalInterface::show_instructions_screen()
{
    clear();
    int start_x, start_y;

    start_y = _center_y - 2;
    start_x = _center_x - 30;

    mvprintw(start_y,     start_x, "1. You will play 3 rounds, each with a new word to guess.");
    mvprintw(start_y + 2, start_x, "2. For every correct guess, you earn 1000 points.");
    mvprintw(start_y + 4, start_x, "3. If you fail to guess the word, 1000 points will be deducted.");
    mvprintw(start_y + 6, start_x, "* Please make sure you have speakers'volume turned on:).");

    mvprintw(start_y + 8, _center_x - 5, "Good luck!");

    refresh();
    getch();
}
void GraphicalInterface::show_end_screen(unsigned short score,
        const char * username)
{
    clear();
    int start_x, start_y;

    start_y = _center_y - 2;
    start_x = _center_x - 30;

    mvprintw(start_y,     start_x, "Congrats! %s", username);
    mvprintw(start_y + 2, start_x, "This is your score: %u", score);
    mvprintw(start_y + 6, _center_x - 5, "Better luck next time!");

    refresh();

    noecho();
    getch();
    echo();
}
void GraphicalInterface::uninitialize_ncurses() {
    endwin();
}
void GraphicalInterface::show_round_interface(const char * current_word, unsigned int word_length, unsigned int number_of_mistakes)
{
    clear();

    int word_width = word_length * 2 - 1;
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
void GraphicalInterface::show_win_interface(unsigned int round_number, 
        const char * word, 
        unsigned short score)
{
    clear();
    
    int x1, x2, y1, y2;
    char score_buf[GENERAL_MAX_SIZE], line1[GENERAL_MAX_SIZE];

    snprintf(score_buf, sizeof(score_buf), "Score: %u", score);
    mvprintw(0, _max_x - (int) strlen(score_buf) - 1, "%s", score_buf);

    snprintf(line1, sizeof(line1),
             "Round %u. The word is %s.", round_number, word);
    y1 = _max_y / 2 - 1;
    x1 = (_max_x - (int) strlen(line1)) / 2;
    mvprintw(y1, x1, "%s", line1);

    const char* line2 = "Congrats!";
    y2 = y1 + 2;
    x2 = (_max_x - (int) strlen(line2)) / 2;
    mvprintw(y2, x2, "%s", line2);

    refresh();
}
void GraphicalInterface::show_fail_interface(unsigned int round_number, 
        const char * word, 
        unsigned short score, 
        const char * hint)
{
    clear();
    
    int x1, x2, y1, y2;
    char line1[GENERAL_MAX_SIZE], line2[GENERAL_MAX_SIZE];

    snprintf(line1, sizeof(line1),
             "Round %u. The word: %s.", round_number, word);
    
    if(hint) {
        memset(&line2, 0, GENERAL_MAX_SIZE);
        snprintf(line2, sizeof(line2),
                 "Next Round... %s", hint);
        y2 = _max_y / 2 + 1;
        x2 = (_max_x - (int) strlen(line2)) / 2;
    }

    y1 = _max_y / 2 - 1;
    x1 = (_max_x - (int) strlen(line1)) / 2;

    mvprintw(y1, x1, "%s", line1);

    if(hint) {
        mvprintw(y2, x2, "%s", line2);
    }

    char score_buf[GENERAL_MAX_SIZE];
    snprintf(score_buf, sizeof(score_buf), "Score: %u", score);
    mvprintw(0, _max_x - (int) strlen(score_buf) - 1, "%s", score_buf);

    refresh();
    getch(); 
}
