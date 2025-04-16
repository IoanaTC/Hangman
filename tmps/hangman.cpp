#include <ncurses.h>
#include <cstdlib>
#include <cstring>
#include "hangman.h"
using namespace std;

void init_coordinates(_game_state * game_state) {
    getmaxyx(stdscr, game_state->maxy, game_state->maxx);

    game_state->centerx = game_state->maxx / 2;
    game_state->centery = game_state->maxy / 2;
}
/*bool init_hints(_game_telemetry * game_telemetry) {
    game_telemetry->hint = (char *) malloc (MAX_WORD_LENGTH + 1);
    if(!game_telemetry->hint) {
        printf("Sorry! Error allocating hint!\n");
        return false;
    }
    memcpy(game_telemetry->hint, "Just keep trying. Choose wisely!", MAX_WORD_LENGTH);
    game_telemetry->hint[MAX_WORD_LENGTH] = '\0';
    return true;
}*/
void show_start_screen(_game_state * game_state) {
    clear();

    mvprintw(game_state->centery - 1, game_state->centerx - 15, "Ultimate Hangman game. Welcome!");
    mvprintw(game_state->centery, game_state->centerx - 16, "Disclaimer: You break it, you pay!");
    mvprintw(game_state->centery + 1, game_state->centerx - 12, "Press any key to start...");

    refresh();
    getch();
}
template<>
void get_user_preferences<bool>(const char * question, bool & result) {
    bool user_was_helpful = false;

    char response[4];
    unsigned char response_size = sizeof(response);
    memset(&response, 0, response_size);

    do{
        clear();

        mvprintw(0, 1, question);
        curs_set(1);
        getnstr(response, response_size);
        curs_set(0);
        
        if(!strcmp(response, "yes")) {
            result = true;
            user_was_helpful = true;
        }
        if(!strcmp(response, "no")) {
            user_was_helpful = true;
        }
    } while(!user_was_helpful);
}
template<unsigned int N>
void get_user_preferences(const char * question, char (&result)[N]) {
    clear();

    mvprintw(0, 1, question);
    curs_set(1);
    getstr(result);
    curs_set(0);
}
int main(int argc, char ** argv) {
    /* initialise ncurses */
    initscr();
    cbreak();

    /* initialize game state */
    _game_state game_state;
    memset(&game_state, 0, sizeof(_game_state));
    game_state.guesses_left = DEFAULT_TRIES;
    init_coordinates(&game_state);

    show_start_screen(&game_state);

    /* initialize game telemetry with input from user */
    _game_telemetry game_telemetry;
    memset(&game_telemetry, 0, sizeof(_game_telemetry));
    /*if(!init_hints(&game_telemetry)) {
        return EC_FAILURE;
    }*/

    //get_user_preferences("Do you need any hints? (yes / no): ", game_telemetry.disable_hints);
    //get_user_preferences("Do you want to save your stats to a file? (yes / no): ", game_telemetry.save_stats);
    /*if(game_telemetry.save_stats) {
        get_user_preferences("Please enter filename: ", game_telemetry.stats_filename);
    }*/

    get_user_preferences("What is your preferred username?: ", game_telemetry.username);
    get_user_preferences("Please provide a dictionary of words: ", game_telemetry.dictionary_file);
    /* initialize word list from user-given dictionary */

    /* DEBUG */
    printf("game_telemetry.disable_hints = %d\n", game_telemetry.disable_hints);
    printf("game_telemetry.save_stats = %d\n", game_telemetry.save_stats);
    if(game_telemetry.save_stats)
        printf("game_telemetry.stats_filename = %s\n", game_telemetry.stats_filename);
    printf("game_telemetry.username = %s\n", game_telemetry.username);
    printf(game_telemetry.username);
    
    endwin();    
    return EC_SUCCESS;
}


