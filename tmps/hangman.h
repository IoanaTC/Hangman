#define EC_SUCCESS 0
#define EC_FAILURE -1

#define MAX_WORD_LENGTH 32
#define DEFAULT_TRIES 6
#define ALPHABET_SIZE 36

/* List of vulnerabilities:
 * 1. INTEGER UNDERFLOW : game_telemtry->score (EASY)
 *      -> game mechanics: - upon feeding a wrong letter N points will be substracted from score
 *      (without checking for underflow)
 *                         - if the correct letter has been chosen, then M points will be added as well
 *      -> if score manages to get too high (either by underflow, or fair and square), it will produce an *effect*
 * 
 * * *OBS: The words will be read from a dictionary file given as input by the user before a game starts.
 *         They will be stored and feed to the user randomly for the remaining rounds of the current game.
 * 2. STRING FORMAT :  game_state->current_guess_word[MAX_WORD_LENGTH] (MODERATE)
 *      -> there will be no word pre-processing, no check if each letter is a valid letter from the alphabet 
 *      -> only filter words by MAX_WORD_LENGTH to remain within bounds
 *          - this vulnerability can lead to memory disclosure
 *
 * 3. BUFFER OVERFLOW : game_telemetry->username (EASY)
 *      -> there will be no OOB-write check for this variable
 *      -> can lead to altering data above it
 *
 * 4. USE AFTER FREE : game_telemetry->hints_list (MODERATE)
 *      -> game mechanics: initially, the option game_telemetry->disable_hints will be false
 *          - dynamically allocated strings (hints) will be shown on the screen
 *      -> the user can choose to able or disable this option at game start
 *          - if it disables the option, the hint buffers will be freed
 *          - IMPORTANT: but the pointers will not be nulled out
 *      !-> the user can later make use of the BUFFER OVERFLOW discovered earlier and change this option by writing oob
 *          - if it reenables the option, the application will try to display freed pointers
 *
 * 5. SHARED LIBRARY HIJACKING : user loads a vulnerable dynamic library (HARD)
 *      -> game mechanics: upon winning (*effect*), the elf will dynamically load a shared object by its name ex: "plugin.so"
 *          and execute a funtion
 *      -> the elf will be also compiled with special flags so that the loader also searches the current directory
 *          which can be easily hijacked by an attacker, so shouldn't be loaded from cwd
 *
 * 6. TOCTOU
 */
typedef struct __game_state {
    unsigned int guesses_left;
    
    char current_guess_word[MAX_WORD_LENGTH]; // do not sanitize, string format vuln when show word (4)
    unsigned int guess_word_length;
   
    char wrong_characters[DEFAULT_TRIES];

    int maxx, maxy, centerx, centery;
} _game_state;

typedef struct __game_telemetry {
    bool save_stats;
    bool disable_hints;
    
    char username[MAX_WORD_LENGTH];
    unsigned int score;
    char stats_filename[MAX_WORD_LENGTH];
    char ** hint_list;
    /* disable hint is initially 0, allocates hint
     * turn on option "disable_hints" which frees hint pointer
     * overwrite disable_hints using username username, make it 1 again
     * use after free 
     */
} _game_telemetry;

typedef struct __word_list {
    char ** words;
    unsigned int word_count;
} _word_list;

template<typename T>
void get_user_preferences(const char * question, T& result);

//bool create_word_list();
//unsigned int get_suitable_index();
//unsigned int choose_word();

