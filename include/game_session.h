#pragma once

#include "debug.h"
#include "gui.h"

#include <ncurses.h>
#include <bits/stdc++.h>
#include <dlfcn.h>

using namespace std;

#define MAX_WORD_LENGTH 32
#define MIN_WORD_LENGTH 5

#define MAX_FILE_PATH_LENGTH 256
#define MAX_READ_SIZE (1 * 1024 * 1024)

#define MAX_STRING_SIZE 1024
#define MAX_NUMBER_OF_HINTS 3

#define MAX_NUMBER_OF_ROUNDS 3
#define MAX_NUMBER_MISTAKES 7

#define PENALTY_POINTS 10000

typedef struct _word {
    char * word;
    unsigned int length;
} word;

class GameRound {
private:
    char * _current_word; 
    word * _answer;
    set<char> _mistakes;
    bool _result;

    bool _show_hints;

    GraphicalInterface * _gui_instance;
public:
    GameRound()=delete;

    GameRound(word * complete_word, bool show_hints = true);
    ~GameRound();

    void guess(unsigned int round_number);
    bool get_result();
};

class GameSession {
private:
    char _username[MAX_WORD_LENGTH];
    bool _show_hints;

    word ** _words_dictionary; 
    unsigned int _number_of_chosen_words;

    static char ** _hint_phrases;

    static unsigned short _score;
    char _save_score_file[MAX_FILE_PATH_LENGTH];

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
    void save_score_file();
    
    char * get_username();

    static unsigned short GetScore();
    static void AddScore(short value);

    static void InitHints();
    static char * GetHint(unsigned int index);

    void play_winning_sound();
};

