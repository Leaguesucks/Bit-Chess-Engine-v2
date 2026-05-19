#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

#include "headers/General.h"
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

    Game game("8/B1R5/1p4R1/4P2k/2n3bp/2r4r/Qn1b1K2/8 w - - 0 1");
    // printBoard(game.getBoard()->allPositions[W], std::cerr);
    // printBoard(game.getBoard()->allPositions[B], std::cerr);
    printBoard(game.getCalBoard()->moves[C7], std::cerr);
    printBoard(game.getCalBoard()->moves[G6], std::cerr);

    // UCI uci(&game);

    // uci.connect();


    return 0;
}
