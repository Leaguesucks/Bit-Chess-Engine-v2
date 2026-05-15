#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

#include "../debug/debug.h"
#include "headers/General.h"
#include "headers/BitManipulation.h"
#include "headers/Sliders.h"
#include "headers/Magic.h"
#include "headers/LookupTable.h"
#include "headers/Game.h"
#include "headers/UCI.h"

int main() {
    std::cout.setf(std::ios::unitbuf);

    // Game game(BEGIN);
    // UCI uci(&game);

    // uci.connect();

    Game game("2k2n2/8/1Nrb1r1B/1P2B3/2RP2q1/2Q5/2b5/KN6 w - - 0 1");
    std::cout << game.getBoard()->board << std::endl;
    std::cout << game.toFEN() << std::endl;
    return 0;
}
