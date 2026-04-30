#pragma once

#include <string.h>

#include "General.h"
#include "LookupTable.h"
#include "Game.h"
#include "BitManipulation.h"

/**
 * @brief Responsible for the move generation of every piece on the board. Check castling 
 *        right, check/checkmate, if a piece is pinned, etc.
 * @note Make sure LookupTable and BitManipulation are initialized
 */
namespace MoveGen {
    extern bool init;

    /**
     * @brief Initialize any required data, such as the lookup table
     */
    void initialize();

    /**
     * @brief Generate moves for each pieces on each side and update it on the bit board
     * 
     * @param bb The bit board that contains the current information of the game
     * @param piece Which piece to generate the moves for
     * @param square The square that this piece is on
     */
    void genMoves(BitBoard* bb, int piece, int square);
}