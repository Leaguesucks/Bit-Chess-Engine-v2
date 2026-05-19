#pragma once

#include <cstring>
#include <math.h>

#include "General.h"
#include "Sliders.h"
#include "BitManipulation.h"

namespace LookupTable {
    const u64 BISHOP_MAGIC_NUMBERS[64] = {
        0x420c80100408202ULL,
        0x1204311202260108ULL,
        0x2008208102030000ULL,
        0x24081001000caULL,
        0x488484041002110ULL,
        0x1a080c2c010018ULL,
        0x20a02a2400084ULL,
        0x440404400a01000ULL,
        0x8931041080080ULL,
        0x200484108221ULL,
        0x80460802188000ULL,
        0x4000090401080092ULL,
        0x4000011040a00004ULL,
        0x20011048040504ULL,
        0x2008008401084000ULL,
        0x102422a101a02ULL,
        0x2040801082420404ULL,
        0x8104900210440100ULL,
        0x202101012820109ULL,
        0x248090401409004ULL,
        0x44820404a00020ULL,
        0x40808110100100ULL,
        0x480a80100882000ULL,
        0x184820208a011010ULL,
        0x110400206085200ULL,
        0x1050010104201ULL,
        0x4008480070008010ULL,
        0x8440040018410120ULL,
        0x41010000104000ULL,
        0x4010004080241000ULL,
        0x1244082061040ULL,
        0x51060000288441ULL,
        0x2215410a05820ULL,
        0x6000941020a0c220ULL,
        0xf2080100020201ULL,
        0x8010020081180080ULL,
        0x940012060060080ULL,
        0x620008284290800ULL,
        0x8468100140900ULL,
        0x418400aa01802100ULL,
        0x4000882440015002ULL,
        0x420220a11081ULL,
        0x401a26030000804ULL,
        0x2184208000084ULL,
        0xa430820a0410c201ULL,
        0x640053805080180ULL,
        0x4a04010a44100601ULL,
        0x10014901001021ULL,
        0x422411031300100ULL,
        0x824222110280000ULL,
        0x8800020a0b340300ULL,
        0xa8000441109088ULL,
        0x404000861010208ULL,
        0x40112002042200ULL,
        0x2141006480b00a0ULL,
        0x2210108081004411ULL,
        0x2010804070100803ULL,
        0x7a0011010090ac31ULL,
        0x18005100880400ULL,
        0x8010001081084805ULL,
        0x400200021202020aULL,
        0x4100342100a0221ULL,
        0x404408801010204ULL,
        0x6360041408104012ULL
    };

    const u64 ROOK_MAGIC_NUMBERS[64] = {
        0x8080008118604002ULL,
        0x4040100040002002ULL,
        0x80100018e00380ULL,
        0x100041002200900ULL,
        0x200020008100420ULL,
        0x4100040002880100ULL,
        0x80008002000100ULL,
        0x8100014028820300ULL,
        0x860802080004008ULL,
        0x112004081020024ULL,
        0x1042002010408200ULL,
        0x410010000b0020ULL,
        0x20800800800400ULL,
        0x4808026000400ULL,
        0x820800100800200ULL,
        0xd43000a00a04900ULL,
        0x4080818000400068ULL,
        0x20818040002005ULL,
        0xa0010018410020ULL,
        0x8010004008040041ULL,
        0x28008008800400ULL,
        0x809010002080400ULL,
        0x1040240048311230ULL,
        0x88020000d28425ULL,
        0x1480004440002010ULL,
        0x2020400440201000ULL,
        0x2000200080100080ULL,
        0x1400280280300080ULL,
        0x4028002500181100ULL,
        0x8040040080800200ULL,
        0x800020400108108ULL,
        0x3041120004408cULL,
        0x80804008800020ULL,
        0x4010002000400040ULL,
        0x2000100080802000ULL,
        0x8300810804801000ULL,
        0x8011001205000800ULL,
        0x810800601800400ULL,
        0x4301083214000150ULL,
        0x204026458e001401ULL,
        0x40204000808000ULL,
        0x8001008040010020ULL,
        0x8410820820420010ULL,
        0x1003001000090020ULL,
        0x804040008008080ULL,
        0x12000810020004ULL,
        0x1000100200040208ULL,
        0x430000a044020001ULL,
        0x280009023410300ULL,
        0xe0100040002240ULL,
        0x200100401700ULL,
        0x2244100408008080ULL,
        0x8000400801980ULL,
        0x2000810040200ULL,
        0x8010100228810400ULL,
        0x2000009044210200ULL,
        0x4080008040102101ULL,
        0x40002080411d01ULL,
        0x2005524060000901ULL,
        0x502001008400422ULL,
        0x489a000810200402ULL,
        0x1004400080a13ULL,
        0x4000011008020084ULL,
        0x26002114058042ULL
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