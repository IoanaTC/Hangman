#include "hangman.h"
#include "debug.h"
#include "gui.h"

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
    gui->uninitialize_ncurses();
    /* gather user data for current game session */
    return 0;
}
