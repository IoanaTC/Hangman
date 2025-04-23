#pragma once

#include "debug.h"

#include <ncurses.h>
#include <bits/stdc++.h>

using namespace std;

#define MAX_WORD_LENGTH 32
#define MAX_FILE_PATH_LENGTH 256
#define MAX_NUMBER_OF_ROUNDS 3

#define MAX_READ_SIZE (1 * 1024 * 1024)

#define MIN_WORD_LENGTH 5

/* game session information -> word dictionary, username, score, ...
 * game match information -> current word, letters mistaken, ...
 */

typedef struct _word {
    string word;
    unordered_multiset<char> all_letters;
    set<char> mistakes;
} word;

// singleton
class GameSession {
private:
    bool _show_hints;                   // ask
    char _username[MAX_WORD_LENGTH];    // ask

    FILE * _word_file;                  // ask
    word ** _words_dictionary; 

    char ** _hint_phrases;

    unsigned short _score;
    FILE * _save_score_file;

    //GameRound ** _game_rounds;

    GameSession();
    static GameSession * _game_session;
    
    template<typename T>
    void GetUserPreferences(const char * question, T& result);

    template<unsigned int N>
    void GetUserPreferences(const char * question, char (&result)[N]);

    bool CreateWordsDictionary(FILE * word_file = nullptr);

public:
    GameSession(const GameSession &) = delete;
    void operator=(const GameSession &) = delete;

    static GameSession * GetInstance();
    ~GameSession();
    
    /* set & get */
};

