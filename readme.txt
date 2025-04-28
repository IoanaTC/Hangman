Hangman CPP

A classic Hangman word-guessing game implemented in C++ with a simple ncurses-based UI.

GAME INTRODUCTION
-----------------
Guess the hidden word one letter at a time.
• You get up to 6 mistakes before the hangman is complete.
• Correct guesses reveal letters in place; wrong guesses add to the hangman drawing.
• Earn 1000 points for each word you guess correctly; lose 1000 points if you fail.

BUILD SYSTEM
------------
From your project root, run:
    make clean
    make BUILD=debug
    make BUILD=release
This produces the executable (hangman) and plugin (win_sound.so) in the project root directory.

WORD DICTIONARY
---------------
When you start the game you’ll be prompted:
    Please provide a word dictionary file path:
Point it at any newline-separated list of lowercase words!

Provide anything else and you’ll be chasing invisible letters!

An example dictionary is included at:
    ./real_words.txt

RUNNING THE GAME
----------------
1. Build as above.
2. From project root, launch:
       ./hangman
3. Follow the prompts for username, hints preference, and your dictionary path.
4. Enjoy the game!

ENJOY!
------
