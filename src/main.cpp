#include "hangman.h"
#include "debug.h"
#include "gui.h"

using namespace std;

int main()
{
    debug_printf("salutare");
    const GraphicalInterface * gui = GraphicalInterface::GetInstance();
    if(!gui) {
        debug_printf("Graphical Interface could not be constructed");
        return EC_ERROR;
    }
    return 0;
}
