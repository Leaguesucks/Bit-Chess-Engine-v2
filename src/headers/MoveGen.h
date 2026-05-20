#pragma once

#include <cstring>

#include "Global.h"
#include "LookupTable.h"
#include "BitManipulation.h"

/**
 * @brief Responsible for the move generation of every piece on the board. Check castling 
 *        right, check/checkmate, if a piece is pinned, etc.
 * @note Make sure LookupTable and BitManipulation are initialized
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
     */
    void genMoves(BitBoard& bb, CalBoard& cb, int piece, int square);

    /**
     * @brief Generate moves and captures for the pawn
     * 
     * @param bb The bit board that holds the current information of the game
     * @param cb The calculation board to print the moves and captures information
     * @param square The current square of this pawn
     * 
     * @return A raw (possible) attack masks of the pawn
     */
    u64 genPawnMove(BitBoard&, CalBoard& cb, int square);

    /**
     * @brief Generate "raw" moves for the rooks and bishops
     * 
     * @param bb The bit board that holds the current information of the game
     * @param bishop True if the piece being evaluated is a bishop, false otherwise
     * @param square The current square the rook/bishop is on
     * @return <moves, captures, rawMoves> map for the rook/bishop
     * 
     * @note Here, we do not return directly into Calboard to avoid coupling behavior since
     *       the move calculation for the queen require calling this function twice
     */
    maskReturn genRookBishopMove(BitBoard& bb, bool bishop, int square);

    /**
     * @brief Masks any pinned pieces
     * 
     * @param bb The bit board that holds the current information of the game
     * @param cb The calculation board to return the masked pin pieces
     */
    void setPin(BitBoard& bb, CalBoard& cb);

    /**
     * @brief Helper functions to find the pin pieces
     * 
     * @param cb The calculation board to return the masked pin pieces
     * @param kingSq The current square of the allies' king
     * @param allies The allies occupancy map
     * @param pinners The map of the enemies' rook-queen or bishop-queen whoese x-ray
     *                attacks reach the allies' king
     */
    void setRookBishopPin(CalBoard& cb, int kingSq, u64 allies, u64 pinners);

    /**
     * @brief Find the moves and captures for the king, include castling
     * 
     * @param bb The bit board that holds the information of the game
     * @param cb The calculation board to return the moves and
     * @param square The current square the king reside on. Used to avoid the popBit operation
     * 
     * @return The raw move of the king
     */
    u64 genKingMove(BitBoard& bb, CalBoard& cb, int square);

    /**
     * @brief Set/unset the castling flag
     * 
     * @param cb The calculation board that contains the castling flag
     * @param side The side to castle
     * @param castleSide King side or Queen side
     * @param set True to set the flag or false to unset it
     */
    void setCastleFlag(CalBoard& cb, int side, int castleSide, bool set);

    /**
     * @brief Determine the actions to take when undercheck
     * 
     * @param bb The bit board that holds current information of the game
     * @param cb The calculation board to return the calculation
     * @param square The current square this piece reside on
     * @param piece Which piece it is
     */
    void handleCheck(BitBoard& bb, CalBoard& cb, int square, int piece);
}