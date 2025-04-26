#pragma once

#include "debug.h"
#include "gui.h"

#include <ncurses.h>
#include <bits/stdc++.h>

using namespace std;

#define MAX_WORD_LENGTH 32
#define MIN_WORD_LENGTH 5

#define MAX_FILE_PATH_LENGTH 256
#define MAX_READ_SIZE (1 * 1024 * 1024)

#define MAX_NUMBER_OF_ROUNDS 3
#define MAX_NUMBER_MISTAKES 7


/* game session information -> word dictionary, username, score, ...
 * game match information -> current word, letters mistaken, ...
 */

typedef struct _word {
    char * word;
    unsigned int length;
} word;

class GameRound {
private:
    char * _current_word; 
    word * _answer;
    set<char> _mistakes;

    GraphicalInterface * _gui_instance;
public:
    GameRound()=delete;

    GameRound(word * complete_word);
    ~GameRound();

    void guess();
};

// singleton
class GameSession {
private:
    bool _show_hints;                   // ask
    char _username[MAX_WORD_LENGTH];    // ask

    word ** _words_dictionary; 
    unsigned int _number_of_chosen_words;

    char ** _hint_phrases;

    unsigned short _score;
    FILE * _save_score_file;

    GameSession();
    static GameSession * _game_session;
    
    template<typename T>
    void GetUserPreferences(const char * question, T& result);

    template<unsigned int N>
    void GetUserPreferences(const char * question, char (&result)[N]);

    bool CreateWordsDictionary(FILE * word_file = nullptr);
    set<pair<char *, unsigned int>> filter_words_from_buffer(const char * buffer, unsigned int size);

public:
    GameSession(const GameSession &) = delete;
    void operator=(const GameSession &) = delete;

    static GameSession * GetInstance();
    ~GameSession();
    
    void StartGame();
    /* set & get */
};

