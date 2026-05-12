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
     * @brief Generate moves for each pieces on each side 
     *        and update it on the bit board
     * 
     * @param bb The bit board that holds the current information of the game
     * @param cb The calculation board to print the moves and captures information
     * @param piece Which piece to generate the moves for
     * @param square The square that this piece is on
     */
    void genMoves(BitBoard& bb, CalBoard& cb, int piece, int square);

    /**
     * @brief Generate "raw" moves for the rooks and bishops
     * 
     * @param bb The bit board that holds the current information of the game
     * @param cb The calculation board to print the moves and captures information
     * @param bishop True if the piece being evaluated is a bishop, false otherwise
     * @param square The current square the rook/bishop is on
     */
    void genRawRookBishopMove(BitBoard& bb, CalBoard&cb, bool bishop, int square);
}