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
#include "headers/GameData.h"
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

    // Game game("rnbqkbnr/pppp1ppp/8/4p2Q/4P3/8/PPPP1PPP/RNB1KBNR b KQkq - 1 2");
    Game game(BEGIN);
    UCI uci(&game);
    // MoveGen::setPin(*game.getBoard(), *game.getCalBoard(), Black);
    // printBoard(game.getCalBoard()->pinned[Black], std::cerr);
    // printBoard(game.getCalBoard()->moves[F7], std::cerr);

    // std::cerr << game.makeMove(uci.buildMove("Nb1f3")) << std::endl;

    uci.connect();

    // printBoard(game.getBoard()->allPositions[White], std::cerr);
    // std::cerr << game.toFEN() << std::endl;

    // for (int i = 0; i < 64; i++) {
    //     if (game.getBoard()->board[i] == EMPTY)
    //         std::cerr << 0;
    //     else
    //         std::cerr << game.getBoard()->board[i];
    // }
    // std::cerr << std::endl;
    // for (int i = 0; i < 8; i++)
    //     for (int j = 0; j < 8; j++)
    //         std::cerr << j;

    // std::cerr << std::endl;
    return 0;
}
