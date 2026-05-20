#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

#include "headers/Global.h"
#include "headers/BitManipulation.h"
#include "headers/Sliders.h"
#include "headers/Magic.h"
#include "headers/LookupTable.h"
#include "headers/MoveGen.h"
#include "headers/Game.h"
#include "headers/UCI.h"

void initialize() {
    std::cout.setf(std::ios::unitbuf);
    BitManipulation::initialize();
    Sliders::initialize();
    LookupTable::initialize();
    MoveGen::initialize();
}

int main() {
    initialize();

    Game game(BEGIN);
    // printBoard(game.getBoard()->allPositions[0], std::cerr);
    // printBoard(game.getBoard()->allPositions[1], std::cerr);
    // game.setFEN("5n2/2B2nk1/6PN/1Br1rP2/Rb4Q1/2Npq3/1K6/8 w - - 0 1");
    // printBoard(game.getCalBoard()->moves[B1], std::cerr);
    // printBoard(game.getCalBoard()->moves[B7], std::cerr);
    UCI uci(&game);

    uci.connect();



    return 0;
}
