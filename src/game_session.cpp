#include "game_session.h"

GameSession * GameSession::_game_session = nullptr;
unsigned short GameSession::_score = 0;
char ** GameSession::_hint_phrases = nullptr;

template<>
void GameSession::GetUserPreferences(const char * question, bool & result) {
    bool user_was_helpful = false;

    char response[4];
    unsigned char response_size = sizeof(response) - 1;
    memset(&response, 0, response_size);

    do{
        clear();

        mvprintw(0, 1, question);
        curs_set(1);
        echo();
        getnstr(response, response_size);
        response[response_size] = '\0';
        noecho();
        curs_set(0);
        
        if(!strcmp(response, "yes")) {
            result = true;
            user_was_helpful = true;
        }
        if(!strcmp(response, "no")) {
            result = false;
            user_was_helpful = true;
        }
    } while(!user_was_helpful);
}

template<unsigned int N>
void GameSession::GetUserPreferences(const char * question, char (&result)[N]) {
    clear();

    mvprintw(0, 1, question);
    curs_set(1);
    echo();
    getstr(result);
    noecho();
    curs_set(0);
}

set<pair<char *, unsigned int>> GameSession::filter_words_from_buffer (const char * buffer, unsigned int size)
{
    set<pair<char *, unsigned int>> dictionary;

    const char * word_start = buffer;
    for(unsigned int i = 0; i < size - 1; i++) {
        if(isspace(buffer[i]) && buffer[i] != '\0') {
            unsigned int word_length = &buffer[i] - word_start;
            if(word_start != &buffer[i]
                    && word_length > MIN_WORD_LENGTH
                    && word_length < MAX_WORD_LENGTH) {

                char * word = (char *) malloc(MAX_WORD_LENGTH);
                memset(word, 0, MAX_WORD_LENGTH);
                if(word) {
                    strncpy(word, word_start, word_length);

                    debug_printf("word = %s", word);
                    dictionary.insert({word, word_length + 1});
                    this->_number_of_chosen_words += 1;
                }
            }
            word_start = &buffer[i + 1];
            if(dictionary.size() >= MAX_NUMBER_OF_ROUNDS) {
                break;
            }
        }
    }
    return dictionary;
}

void free_extracted_words(set<pair<char *, unsigned int>> extracted_words)
{
    for (auto it = extracted_words.begin(); it != extracted_words.end(); ) {
        free(it->first);
        it = extracted_words.erase(it);
    }
}

bool GameSession::CreateWordsDictionary(FILE * word_file)
{
    char * file_content = (char *) malloc(MAX_READ_SIZE);
    if(!file_content) {
        debug_printf("No memory error upon reading file content");
        return false;
    }
    memset(file_content, 0, MAX_READ_SIZE);

    unsigned int bytes_read = fread(file_content, 1, MAX_READ_SIZE - 1, word_file);
    if(!bytes_read) {
        debug_printf("Word file could not be processed");
        
        free(file_content);
        file_content = nullptr;
        return false;
    }
    set<pair<char *, unsigned int>> extracted_words = filter_words_from_buffer(file_content, bytes_read);
    free(file_content);
    file_content = nullptr;
    
    if(!this->_number_of_chosen_words) {
        debug_printf("Choose a better words dictionary!");
        free_extracted_words(extracted_words);
        return false;
    }
    unsigned int words_dictionary_size = (sizeof(word *) + sizeof(word) + MAX_WORD_LENGTH) * this->_number_of_chosen_words;
    this->_words_dictionary = (word **) malloc(words_dictionary_size);

    if(!this->_words_dictionary) {
        debug_printf("Words Dictionary could not be created");

        free_extracted_words(extracted_words);
        return false;
    }
    memset(this->_words_dictionary, 0, words_dictionary_size);

    pair<char *, unsigned int> current_word = * extracted_words.begin();
    for(unsigned int round = 0; round < this->_number_of_chosen_words; round ++) {

        this->_words_dictionary[round] = (word *)((char *)this->_words_dictionary 
                + this->_number_of_chosen_words * sizeof(word *) 
                + round * sizeof(word));

        this->_words_dictionary[round]->word = (char *)((char *)this->_words_dictionary 
                + this->_number_of_chosen_words * (sizeof(word *) + sizeof(word)) 
                + round * MAX_WORD_LENGTH);

        memcpy(((word *)this->_words_dictionary[round])->word, current_word.first, current_word.second - 1);
        this->_words_dictionary[round]->word[current_word.second] = '\0';
        this->_words_dictionary[round]->length = current_word.second - 1;

        current_word = *next(extracted_words.begin(), round + 1);
    }
    free_extracted_words(extracted_words);
    return true;
}
unsigned short GameSession::GetScore()
{
    return _score;
}
void GameSession::AddScore(short value)
{
    _score += value;
}
void GameSession::InitHints()
{
    unsigned int hints_size = MAX_NUMBER_OF_HINTS * (sizeof(char *) + MAX_STRING_SIZE);
    _hint_phrases = (char **) malloc(hints_size);

    if(!_hint_phrases) {
        debug_printf("Game Session hint phrases could not be allocated");
        throw runtime_error("Game Session hint phrases could not be allocated");
    }
    memset(_hint_phrases, 0, hints_size);
    for(unsigned i = 0; i < MAX_NUMBER_OF_HINTS; i++) {
        _hint_phrases[i] = (char *)_hint_phrases 
            + MAX_NUMBER_OF_HINTS * sizeof(char *)
            + i * MAX_STRING_SIZE;
    }

    strncpy(_hint_phrases[0], "Keep going! You are doing great! Every guess brings you one step closer.", MAX_STRING_SIZE - 1);
    _hint_phrases[0][MAX_STRING_SIZE - 1] = '\0';
    strncpy(_hint_phrases[1], "Do not give up now! You haveve already uncovered some letters! Trust your instincts.", MAX_STRING_SIZE - 1);
    _hint_phrases[1][MAX_STRING_SIZE - 1] = '\0';
    strncpy(_hint_phrases[2], "Great work so far! Keep up the momentum and crack this word!", MAX_STRING_SIZE - 1);
    _hint_phrases[2][MAX_STRING_SIZE - 1] = '\0';

    debug_printf("%s", _hint_phrases[0]);
    debug_printf("%s", _hint_phrases[1]);
    debug_printf("%s", _hint_phrases[2]);
}
char * GameSession::GetHint(unsigned int index)
{
    return _hint_phrases[index];
}
GameSession::GameSession() : _show_hints(false), _number_of_chosen_words(0)
{
    InitHints();

    GetUserPreferences("What is your username? : ", this->_username);
    GetUserPreferences("Do you want any hints? (yes / no) : ", this->_show_hints);

    if(!this->_show_hints) {
        debug_printf("free hints: %x", this->_show_hints);
        free(_hint_phrases);
    }

    char _filename[MAX_FILE_PATH_LENGTH];
    memset(_filename, 0, MAX_FILE_PATH_LENGTH);

    GetUserPreferences("Please provide a word dictionary file path : ", _filename);
    FILE * f = fopen(_filename, "r");
    if(!f || !CreateWordsDictionary(f)) {
        debug_printf("Word Dictionary file could not pe processed");
        if(f) fclose(f);
        if(_hint_phrases) {
            free(_hint_phrases);
            _hint_phrases = nullptr;
        }
        throw runtime_error("Word Dictionary file could not pe processed");
    }
    GetUserPreferences("Please provide file path for your final result : ", _filename);
    FILE * _save_score_file = fopen(_filename, "w+");
    if(!_save_score_file) {
        debug_printf("Could not open score file");
    }
    /* TODO: get score file name  and open it*/
    if(f) fclose(f);    
}

void GameSession::StartGame()
{
    for(unsigned int i = 0; i < _number_of_chosen_words; i++) {
        GameRound _game_round(_words_dictionary[i], this->_show_hints);

        clear();
        _game_round.guess(i);        

        if(_game_round.get_result()) {
            play_winning_sound();
        }
    }
}
GameSession * GameSession::GetInstance()
{
    if(!_game_session) {
        try {
            _game_session = new GameSession();
            if(!_game_session) {
                debug_printf("GameSession constructor has failed");
                return nullptr;
            }
        } catch (const std::exception& ex) {
            debug_printf("GameSession Consructor error");
            return 0;
        }
    }
    return _game_session;
}
GameSession::~GameSession()
{
    if(_words_dictionary) {
        free(_words_dictionary);
        _words_dictionary = nullptr;
    }
    if(_hint_phrases) {
        free(_hint_phrases);
        _hint_phrases = nullptr;
    }
    if(_save_score_file) {
        fclose(_save_score_file);
        _save_score_file = nullptr;
    }
}
void GameSession::play_winning_sound()
{
    void * file_handle = dlopen("./winning_sound.so", RTLD_NOW);
    dlerror();
    if(!file_handle) {
        debug_printf("Play winning sound, could not open .si");
        return;
    }
    typedef void (* play_function)();
    play_function function = (play_function) dlsym(file_handle, "play_winning_sound");
    if(!function || dlerror()) {
        debug_printf("Play winning sound, could not find symbol");
        dlclose(file_handle);
        return;
    }
    debug_printf("play funciton");
    function();
    dlclose(file_handle);
}

GameRound::GameRound(word * answer, bool show_hints) : _result(false)
{
    _answer = answer;

    _current_word = (char *) malloc(_answer->length);
    if(!_current_word) {
        debug_printf("Game Round Constructor Error");
        throw runtime_error("Game Round Constructor error");
    }
    memset(_current_word, 0, _answer->length) ;

    _gui_instance = GraphicalInterface::GetInstance();
    if(!_gui_instance) {
        debug_printf("Game Round (Interface) constructor error");
        throw runtime_error("Game Round (Interface) constructor error");
    }
    this->_show_hints = show_hints;
}
void GameRound::guess(unsigned int round_number)
{
    bool mistake = true;
    char character;
    
    cbreak();
    noecho();
    while (_mistakes.size() <= MAX_NUMBER_MISTAKES) {
        if(!strcmp(_current_word, _answer->word)) {
            _result = true;

            GameSession::AddScore((short)PENALTY_POINTS);
            _gui_instance->show_win_interface(round_number,
                    (char *)_answer->word,
                    GameSession::GetScore());
            echo();
            return;
        }
        _gui_instance->show_round_interface(_current_word, _answer->length, _mistakes.size());
        
        character = (char) getch();
        if(character < 'a' || character > 'z') {
            continue;
        }

        if(_mistakes.size() >= MAX_NUMBER_MISTAKES) {
            break;
        }
        mistake = true;
        for(unsigned int letter = 0; letter < _answer->length; letter++) {
            if(_answer->word[letter] == character) {
                mistake = false;
                _current_word[letter] = character;
            }
        }
        if(mistake == true) {
            if(!_mistakes.count(character)) {
                _mistakes.insert(character);
            }
        }
    }
    GameSession::AddScore((short) - PENALTY_POINTS);
    _gui_instance->show_fail_interface(round_number,
            (char *)_answer->word,
            GameSession::GetScore(), 
            this->_show_hints == true ? (char *)GameSession::GetHint(round_number) : nullptr);
    echo();
}
bool GameRound::get_result()
{
    return _result;
}
GameRound::~GameRound()
{
    if(_current_word) {
        free(_current_word);
        _current_word = nullptr;
    }
}
