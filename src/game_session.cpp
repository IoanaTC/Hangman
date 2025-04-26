#include "game_session.h"

GameSession * GameSession::_game_session = nullptr;

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
                    // TODO: lowercase
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
    /* space separated words
     * filter: with more than 5 letters
     * BUG: do not exclude special characters
     */
    
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

    /* create _words_dictionary */
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

GameSession::GameSession() : _show_hints(false), _number_of_chosen_words(0), _score(0)
{
    /* Current Game Session begins
     * the user must respond to a few questions
     * to account for its preferences
     */

    /* 1. get preferred username */
    GetUserPreferences("What is your username? : ", this->_username);
    /* 2. does it want to receive hints */
    GetUserPreferences("Do you want any hints? (yes / no) : ", this->_show_hints);

    /* TODO: init hints - hardcoded */
    
    /* 3. what is the words dictionary */
    char _filename[MAX_FILE_PATH_LENGTH];
    memset(_filename, 0, MAX_FILE_PATH_LENGTH);

    GetUserPreferences("Please provide a word dictionary file path : ", _filename);
    /* open file + create word_dicitonary */
    FILE * f = fopen(_filename, "r");
    if(!f || !CreateWordsDictionary(f)) {
        debug_printf("Word Dictionary file could not pe processed");
        if(f) fclose(f);

        throw runtime_error("WordDictionary could not pe processed");
    }
    if(f) fclose(f);    
    /* TODO: create rounds list 
     * here initialize each round woth a random word from the list
     * create hangman drawing, etc.
     *  TRY CATCH
     */
}

void GameSession::StartGame()
{
    for(unsigned int i = 0; i < _number_of_chosen_words; i++) {
        GameRound _game_round(_words_dictionary[i]);

        clear();
        _game_round.guess();        
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

GameRound::GameRound(word * answer)
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
}
void GameRound::guess()
{
    bool mistake = true;
    char character;
    
    cbreak();
    noecho();
    while (_mistakes.size() <= MAX_NUMBER_MISTAKES) {
        if(!strcmp(_current_word, _answer->word)) {
            break;
        }
        debug_printf("%s", _current_word);
        _gui_instance->show_round_interface(_current_word, _answer->length, _mistakes.size());
        
        character = (char) getch();
        if(character < 'a' || character > 'z') {
            continue;
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
    echo();
}
GameRound::~GameRound()
{
    if(_current_word) {
        free(_current_word);
        _current_word = nullptr;
    }
}
