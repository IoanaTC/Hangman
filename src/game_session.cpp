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

set<string> filter_words_from_buffer (const char * buffer, unsigned int size)
{
    set<string> dictionary;

    const char * word_start = buffer;
    debug_printf("%s", buffer);

    for(unsigned int i = 0; i < size - 1; i++) {
        if(isspace(buffer[i]) && buffer[i] != '\0') {
            if(word_start != &buffer[i] && &buffer[i] - word_start + 1 > MIN_WORD_LENGTH) {
                string word(word_start, &buffer[i]);
                // TODO: lowercase
                debug_printf("word = %s", word);
                dictionary.insert(word);
            }
            word_start = &buffer[i + 1];
        }
    }

    return dictionary;
}
bool GameSession::CreateWordsDictionary(FILE * word_file)
{
    /* space separated words
     * filter: with more than 5 letters
     * BUG: do not exclude special characters
     */
    
    char * file_content = (char *)malloc(MAX_READ_SIZE);
    if(!file_content) {
        debug_printf("No memory error upon reading file content");
        return false;
    }
    memset(file_content, 0, MAX_READ_SIZE);

    unsigned int bytes_read = fread(file_content, 1, MAX_READ_SIZE - 1, word_file);
    if(!bytes_read) {
        debug_printf("Word file could not be processed");
        free(file_content);
        return false;
    }
    debug_printf("just before get dictionary call");
    set<string> extracted_words = filter_words_from_buffer(file_content, bytes_read);
    for(const auto& word : extracted_words) {
   //     debug_printf("%s", word);
    }
    free(file_content);
    return true;
}

GameSession::GameSession() : _show_hints(false), _score(0)
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
     */
}
GameSession * GameSession::GetInstance()
{
    if(!_game_session) {
        // TODO: catch error
        _game_session = new GameSession();
        if(!_game_session) {
            debug_printf("GameSession constructor has failed");
            return nullptr;
        }
    }
    return _game_session;
}
GameSession::~GameSession()
{

}
