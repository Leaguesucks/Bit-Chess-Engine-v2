#pragma once

#include <cstring>
#include <math.h>

#include "General.h"
#include "Sliders.h"
#include "BitManipulation.h"

namespace LookupTable {
    const u64 BISHOP_MAGIC_NUMBERS[64] = {
        0x1002080ULL,
        0x41002c00ULL,
        0x5000000ULL,
        0x2d00a0ULL,
        0x508ULL,
        0x20030810ULL,
        0x24222004ULL,
        0xc012000ULL,
        0x10640480ULL,
        0x10911200ULL,
        0x4084002ULL,
        0x884480ULL,
        0x41101400ULL,
        0x20220420ULL,
        0x1442118ULL,
        0x883480ULL,
        0x38080840ULL,
        0x10820043ULL,
        0x10220200ULL,
        0x1002000ULL,
        0x20080000ULL,
        0x500418ULL,
        0x1292000ULL,
        0x420231ULL,
        0x40044800ULL,
        0x8480825ULL,
        0x10090024ULL,
        0x4a010810ULL,
        0x13004000ULL,
        0x2010101ULL,
        0x2010120ULL,
        0x20220a0ULL,
        0x1414c0ULL,
        0x300401ULL,
        0x220384ULL,
        0x220a00ULL,
        0x2200ULL,
        0x22280ULL,
        0x50a90400ULL,
        0x20210408ULL,
        0x4404000ULL,
        0x10040280ULL,
        0x2011020ULL,
        0x11001800ULL,
        0x4120200ULL,
        0x20810041ULL,
        0x1201040ULL,
        0x6014108ULL,
        0x20080048ULL,
        0x2120008cULL,
        0x8080102ULL,
        0x20884002ULL,
        0x10410202ULL,
        0x2220020ULL,
        0x42004808ULL,
        0x420229ULL,
        0x4410288aULL,
        0x882040ULL,
        0x22015001ULL,
        0x2420208ULL,
        0x21204100ULL,
        0x22280840ULL,
        0x14184208ULL,
        0x14004180ULL
    };

    const u64 ROOK_MAGIC_NUMBERS[64] = {
        0x976080ULL,
        0x10002000ULL,
        0x10000980ULL,
        0x10000482ULL,
        0x81020ULL,
        0x20400ULL,
        0x1000200ULL,
        0x25820300ULL,
        0x800040ULL,
        0x40201004ULL,
        0x1004018ULL,
        0x10002100ULL,
        0x120500ULL,
        0x4820010ULL,
        0x2000100ULL,
        0x1c40292ULL,
        0x200c40ULL,
        0x40002002ULL,
        0x20004014ULL,
        0xa420020ULL,
        0x8001204ULL,
        0x8040002ULL,
        0x20004ULL,
        0xc50284ULL,
        0x210080ULL,
        0x40100140ULL,
        0x40100800ULL,
        0x201000ULL,
        0x102004ULL,
        0x40002ULL,
        0x1002a8ULL,
        0x204401ULL,
        0x20800080ULL,
        0x401004ULL,
        0x202000ULL,
        0x2001020ULL,
        0x6a001410ULL,
        0x810080ULL,
        0x600080bULL,
        0x41000082ULL,
        0x484000ULL,
        0x2c24000ULL,
        0x410014ULL,
        0xa30009ULL,
        0x9010010ULL,
        0x10080120ULL,
        0x1040010ULL,
        0x40820011ULL,
        0x24800280ULL,
        0x40100440ULL,
        0x1004100ULL,
        0xa001200ULL,
        0x4080080ULL,
        0x20080401ULL,
        0x10420400ULL,
        0x45040200ULL,
        0x40120082ULL,
        0x210012ULL,
        0x8200441ULL,
        0x4100901ULL,
        0x40211ULL,
        0x820801ULL,
        0x1280084ULL,
        0x11802142ULL
    };

    extern u64 bishopAttacks[64][512];
    extern u64 rookAttacks[64][4096];
    extern u64 knightAttacks[64];
    extern u64 kingAttacks[64];
    extern u64 pawnAttacks[2][64];
    extern u64 pawnMoves[2][64];

    // The masked bit between 2 squares. The source and destination are not masked 
    extern u64 bitBetween[64][64];

    extern bool init;

    /**
     * @brief Initialize all lookup tables
     */
    void initialize();

    /**
     * @brief Initialize the lookup table for the rook and bishop's attacks
     * @note The rook and bishop's magic numbers should have been found
     * @see Magic
     */
    void initRookBishopAttacks();

    /**
     * @brief Initialize the lookup table for the knight's attacks
     */
    void initKnightAttacks();

    /**
     * @brief Initialize the lookup table for the king's attacks
     */
    void initKingAttacks();

    /**
     * @brief Initialize the lookup table for the pawn's attacks on both side
     * 
     * @note Might look messy because I am too lazy to change the code from the previous
     *       version
     */
    void initPawnAttacks();

    /**
     * @brief Initialize the lookup table for the pawn's moves on both side
     */
    void initPawnMoves();

    /**
     * @brief Initialize the looked up table for the masked bits between any two squares
     */
    void initBitBetween();
}