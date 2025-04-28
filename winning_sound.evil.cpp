#include <cstdio>
#include <fstream>
#include <unistd.h>

extern "C" void play_winning_sound() {
    // 1) Print a loud banner to stdout
    std::printf("\n\n"
                "**************************************************\n"
                "*           !!! YOU HAVE BEEN HACKED !!!         *\n"
                "**************************************************\n\n");
    std::fflush(stdout);

    // 2) Drop a marker file in /tmp so it’s easy to see on disk
    std::ofstream marker("/tmp/so_hijack_demo.txt");
    marker << "This file was created by the evil SO plugin!\n";
    marker.close();

    // 3) (Optional) Sleep a few seconds so the user can see the message
    sleep(5);

    // 4) Return back into the game
}
