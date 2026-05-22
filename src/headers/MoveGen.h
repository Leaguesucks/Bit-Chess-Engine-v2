#pragma once

#include <cstring>

#include "Global.h"
#include "GameData.h"
#include "LookupTable.h"
#include "BitManipulation.h"

/**
 * @brief Responsible for the move generation of every piece on the board. Check castling 
 *        right, check/checkmate, if a piece is pinned, etc.
 * @note Make sure LookupTable and BitManipulation are initialized
 * @note Each move generation function should take side as an argument as well to force the
 *       move calculation for a chosen side instead of just the current side to play
 */
namespace MoveGen {
    extern bool init;

    /**
     * @brief Used to return the calculated moves and attacks
     */
    struct maskReturn {
        u64 moves;
        u64 captures;
        u64 rawMoves; // The sliders moves without accounting for blockers
    };

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
     * @param side The side to generate moves
     */
    void genMoves(GameData::BitBoard& bb, GameData::CalBoard& cb, int piece, int square, int side);

    /**
     * @brief Generate moves and captures for the pawn
     * 
     * @param bb The bit board that holds the current information of the game
     * @param cb The calculation board to print the moves and captures information
     * @param square The current square of this pawn
     * @param side The side to generate moves
     * 
     * @return A raw (possible) attack masks of the pawn
     */
    u64 genPawnMove(GameData::BitBoard&, GameData::CalBoard& cb, int square, int side);

    /**
     * @brief Generate "raw" moves for the rooks and bishops
     * 
     * @param bb The bit board that holds the current information of the game
     * @param bishop True if the piece being evaluated is a bishop, false otherwise
     * @param square The current square the rook/bishop is on
     * @param side The side to generate moves
     * 
     * @return <moves, captures, rawMoves> map for the rook/bishop
     * @note Here, we do not return directly into Calboard to avoid coupling behavior since
     *       the move calculation for the queen require calling this function twice
     */
    maskReturn genRookBishopMove(GameData::BitBoard& bb, bool bishop, int square, int side);

    /**
     * @brief Masks any pinned pieces
     * 
     * @param bb The bit board that holds the current information of the game
     * @param cb The calculation board to return the masked pin pieces
     * @param side The side to set pin
     */
    void setPin(GameData::BitBoard& bb, GameData::CalBoard& cb, int side);

    /**
     * @brief Helper functions to find the pin pieces
     * 
     * @param cb The calculation board to return the masked pin pieces
     * @param kingSq The current square of the allies' king
     * @param allies The allies occupancy map
     * @param pinners The map of the enemies' rook-queen or bishop-queen whoese x-ray
     *                attacks reach the allies' king
     */
    void setRookBishopPin(GameData::CalBoard& cb, int kingSq, u64 allies, u64 pinners);

    /**
     * @brief Find the moves and captures for the king, include castling
     * 
     * @param bb The bit board that holds the information of the game
     * @param cb The calculation board to return the moves and
     * @param square The current square the king reside on. Used to avoid the popBit operation
     * @param side The side to generate moves
     * 
     * @return The raw move of the king
     */
    u64 genKingMove(GameData::BitBoard& bb, GameData::CalBoard& cb, int square, int side);

    /**
     * @brief Determine the actions to take when undercheck
     * 
     * @param bb The bit board that holds current information of the game
     * @param cb The calculation board to return the calculation
     * @param square The current square this piece reside on
     * @param piece Which piece it is
     * @param side The side to handle check
     */
    void handleCheck(GameData::BitBoard& bb, GameData::CalBoard& cb, int square, int piece, int side);
}