#pragma once

#include "Global.h"
#include "BitManipulation.h"

/**
 * @brief Contains functions that generate attacks for the slider pieces like rook and 
 * bishop
 * @note
 * __Relevant Squares__
 *
 * The term *"relevant squares"* refers to the squares along a sliding
 * piece's (rook/bishop) movement rays that influence its attack pattern.
 *
 * Excludes:
 * ```
 * - The piece's current square
 * - Edge squares
 * ```
 *
 * Example: Rook on H1
 *
 * ```
 *          A B C D E F G H
 *
 *      8  0 0 0 0 0 0 0 0  8
 *      7  0 0 0 0 0 0 0 1  7
 *      6  0 0 0 0 0 0 0 1  6
 *      5  0 0 0 0 0 0 0 1  5
 *      4  0 0 0 0 0 0 0 1  4
 *      3  0 0 0 0 0 0 0 1  3
 *      2  0 0 0 0 0 0 0 1  2
 *      1  0 1 1 1 1 1 1 0  1
 *
 *          A B C D E F G H
 * ```
 *
 * OR this:
 * ```
 *         A B C D E F G H  
 *
 *      8  0 0 0 0 0 0 0 0  8 
 *      7  0 0 0 0 0 0 0 1  7 
 *      6  0 0 0 0 0 0 0 1  6 
 *      5  0 0 0 0 0 0 0 1  5 
 *      4  0 0 0 0 0 0 0 1  4 
 *      3  0 0 0 0 0 0 0 1  3 
 *      2  0 0 0 0 0 0 0 1  2 
 *      1  0 0 0 0 0 0 0 0  1 
 *
 *         A B C D E F G H   
 * ```
 * 
 * There are 12 relevant squares, so:
 *
 * ```
 * 2^12 = 4096 variations
 * ```
 *
 * These configurations are used in magic bitboards.
 */
namespace Sliders {
    // Number of relevant squares for each square the bishop resides in
    const u8 NUM_BISHOP_RELEVANT_SQUARES[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
    };

    // Number of relevant squares for each square the rook resides in
    const u8 NUM_ROOK_RELEVANT_SQUARES[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
    };

    extern u64 relevantBishopAttacks[64];
    extern u64 relevantRookAttacks[64];

    extern bool init;

    /**
     * @brief Initialize all required data, such as the lookup tabels
     * @note ALWAYS use this function FIRST
     */
    void initialize();

    /**
     * @return The relevant bishop's free attacks on a given square
     * @note The "free" means there is no blockers
     */
    u64 getRelevantFreeBishopAttacks(int square);

    /**
     * @return The relevant rook's free attacks on a given square
     * @note The "free" means there is no blockers
     */
    u64 getRelevantFreeRookAttacks(int square);

    /**
     * @brief Generate all possible attacks for a bishop on a given square
     * @param blockers The blockers map. No blockers should be on the same square as the
     *                 bishop
     * @param square The current square the bishop is on
     * @return An bit map that contains all possible attacks for the bishop excluding is
     *         current position
     * @note Both the allies and enemies are considers blockers here
     * @note Different from the 'relevant' attacks, this time we account for 
     *       attacks that reach the edges
     */
    u64 getBlockedBishopAttacks(u64 blockers, int square);

    /**
     * @brief Generate all possible attacks for a rook on a given square
     * @param blockers The blockers map. No blockers should be on the same square as the
     *                 rook
     * @param square The current square the rook is on
     * @return An bit map that contains all possible attacks for the rook excluding is
     *         current position
     * @note Both the allies and enemies are considers blockers here
     * @note Different from the 'relevant' attacks, this time we account for
     *       attacks that reach the edges
     */
    u64 getBlockedRookAttacks(u64 blockers, int square);

    /**
     * @brief Find the relevant occupied squares for the rook and bishop
     * @param index From 0 - max 2^12 - 1 = 4095: Represents max 4096 different relevant 
     *              occupied squares of a rook and 2^9 = 512 for a bishop for a given 
     *              rook/bishop's relevant attacks map
     * @param rev_attacks The relevant attacks map of the rook/bishop
     * @return The relevant squares variation of a relevant attacks map given an index
     */
    u64 findRelevantMasks(u32 index, u64 rev_attacks);
}