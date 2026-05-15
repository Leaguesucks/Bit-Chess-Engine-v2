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

    Game game("7k/6b1/2R5/B2n1Pq1/1p6/3Q4/B3bp2/7K w - - 0 1");
    std::cout << game.getBoard()->board << std::endl;
    std::cout << game.toFEN() << std::endl;
    std::cout << game.getBoard()->fifty << " " << game.getBoard()->fly << std::endl;
    return 0;
}
