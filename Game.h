#ifndef TICTACGAME_H
#define TICTACGAME_H
#include "Enum.h"
#include <iostream>

// Main game function.
void tictactoe();

// Main game menu. Returns the number of the choice entered.
int display_menu(std::ostream &os);

// internal function to start game with an ai player. If both players are human, set ai_player to Mark::blank
void start_game(const Mark &ai_player = Mark::blank);
#endif