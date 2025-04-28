#include "hangman.h"
#include "debug.h"
#include "gui.h"
#include "game_session.h"

using namespace std;

int main()
{
    debug_printf("salutare");

    GraphicalInterface * gui = GraphicalInterface::GetInstance();
    if(!gui) {
        debug_printf("Graphical Interface could not be constructed");
        return ERROR;
    }
    gui->initialize_ncurses();
    gui->get_screen_measurements();
    gui->start_interface();
    gui->show_presentation_screen();
    gui->show_instructions_screen();

    GameSession * game_session = GameSession::GetInstance();
    if(!game_session) {
        debug_printf("Game Session could not be constructed");

        gui->uninitialize_ncurses();
        delete gui;

        return ERROR;
    }
    game_session->StartGame();

    game_session->save_score_file();
    gui->show_end_screen(GameSession::GetScore(),
            game_session->get_username());

    gui->uninitialize_ncurses();

    delete gui;
    delete game_session;

    return SUCCESS;
}
