#include "hangman.h"
#include "debug.h"
#include "gui.h"
#include "game_session.h"

using namespace std;

int main()
{
    debug_printf("salutare");

    /* initialize the graphical interface */
    GraphicalInterface * gui = GraphicalInterface::GetInstance();
    if(!gui) {
        debug_printf("Graphical Interface could not be constructed");
        return EC_ERROR;
    }
    gui->initialize_ncurses();
    gui->get_screen_measurements();
    gui->start_interface();
    gui->show_presentation_screen();

    /* gather user data for current game session */
    GameSession * game_session = GameSession::GetInstance();
    if(!game_session) {
        debug_printf("Game Session could not be constructed");
        return EC_ERROR;
    }
    game_session->StartGame();
    gui->uninitialize_ncurses();
    return 0;
}
