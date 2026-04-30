#include "../headers/LookupTable.h"

namespace LookupTable {
    u64 bishopAttacks[64][512];
    u64 rookAttacks[64][4096];
    u64 knightAttacks[64];
    u64 kingAttacks[64];
    u64 pawnMoves[2][64];
    u64 pawnAttacks[2][64];

    u64 bitBetween[64][64];

    bool init = false;
}

void LookupTable::initialize() {
    if (init)
        return;

    BitManipulation::initialize();
    Sliders::initialize();

    initRookBishopAttacks();
    initKnightAttacks();
    initKingAttacks();
    initPawnAttacks();
    initPawnMoves();
    initBitBetween();

    init = true;
}

void LookupTable::initRookBishopAttacks() {
    memset(bishopAttacks, 0, sizeof(bishopAttacks));
    memset(rookAttacks, 0, sizeof(rookAttacks));

    for (int square = 0; square < 64; square++) {
        u32 Num_Bishop_Relevant_Squares_Variations = 1 << Sliders::NUM_BISHOP_RELEVANT_SQUARES[square];
        u32 Num_Rook_Relevant_Squares_Variations = 1 << Sliders::NUM_ROOK_RELEVANT_SQUARES[square];

        // Load up the bishop lookup table
        for (u32 i = 0; i < Num_Bishop_Relevant_Squares_Variations; i++) {
            u64 Relevant_Squares = Sliders::findRelevantSquares(i, Sliders::relevantBishopAttacks[square]);
            u64 LookUp_Index = (Relevant_Squares * BISHOP_MAGIC_NUMBERS[square]) >> (64 - Sliders::NUM_BISHOP_RELEVANT_SQUARES[square]);
            
            if (bishopAttacks[square][LookUp_Index] == 0ULL)
                bishopAttacks[square][LookUp_Index] = Sliders::getBlockedBishopAttacks(Relevant_Squares, square);
        }

        // Load up the rook lookup table
        for (u32 i = 0; i < Num_Rook_Relevant_Squares_Variations; i++) {
            u64 Relevant_Squares = Sliders::findRelevantSquares(i, Sliders::relevantRookAttacks[square]);
            u64 LookUp_Index = (Relevant_Squares * ROOK_MAGIC_NUMBERS[square]) >> (64 - Sliders::NUM_ROOK_RELEVANT_SQUARES[square]);
            
            if (rookAttacks[square][LookUp_Index] == 0ULL)
                rookAttacks[square][LookUp_Index] = Sliders::getBlockedRookAttacks(Relevant_Squares, square);
        }
    }
}

void LookupTable::initKnightAttacks() {
    memset(knightAttacks, 0, sizeof(knightAttacks));

    for (int square = 0; square < 64; square++) {
        int rank = (square / 8) + 1; // Current rank of this square
        int file = (square % 8) + 1; // Current file of this square
        u64 attacks = 0ULL;

        int newRank, newFile;
        if ((newRank = rank - 1) >= 1 && (newFile = file - 2) >= 1) // Short North Long West
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank - 2) >= 1 && (newFile = file - 1) >= 1) // Long North Short West
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank - 2) >= 1 && (newFile = file + 1) <= 8) // Long North Short East
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank - 1) >= 1 && (newFile = file + 2) <= 8) // Short North Long East
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank + 1) <= 8 && (newFile = file + 2) <= 8) // Short South Long East
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank + 2) <= 8 && (newFile = file + 1) <= 8) // Long South Short East
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank + 2) <= 8 && (newFile = file - 1) >= 1) // Long South Short West
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank + 1) <= 8 && (newFile = file - 2) >= 1) // Short South Long West
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
        
        knightAttacks[square] = attacks;
    }
}

void LookupTable::initKingAttacks() {
    memset(kingAttacks, 0 , sizeof(kingAttacks));

    for (int square = 0; square < 64; square++) {
        int rank = (square / 8) + 1; // Current rank of this square
        int file = (square % 8) + 1; // Current file of this square
        u64 attacks = 0ULL;

        int newFile, newRank;
        
        if ((newRank = rank - 1) >= 1 && (newFile = file - 1) >= 1)       // NW
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
        if ((newRank = rank - 1) >= 1)                                    // N
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (file - 1));
        if ((newRank = rank - 1) >= 1 && (newFile = file + 1) <= 8)       // NE
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
        if ((newFile = file + 1) <= 8)                                    // E
            BitManipulation::setBit(&attacks, (rank - 1) * 8 + (newFile - 1));
        if ((newRank = rank + 1) <= 8 && (newFile = file + 1) <= 8)       // SE
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile) - 1);
        if ((newRank = rank + 1) <= 8)                                    // S
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (file - 1));
        if ((newRank = rank + 1) <= 8 && (newFile = file - 1) >= 1)       // SW
            BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
        if ((newFile = file - 1) >= 1)                                    // W
            BitManipulation::setBit(&attacks, (rank - 1) * 8 + (newFile - 1));

        kingAttacks[square] = attacks;
    }
}

void LookupTable::initPawnAttacks() {
    memset(pawnAttacks, 0, sizeof(pawnAttacks));

    for (int square = 0; square < 64; square++) {
        int rank = (square / 8) + 1;
        int file = (square % 8) + 1;

        /* We don't need the first and last row */
        if (rank == 1 || rank == 8)
            continue;

        u64 attacks = 0ULL;

        /* Black pawn's attacks */
        int newRank, newFile;
        if ((newRank = rank - 1) >= 1) {
            if ((newFile = file - 1) >= 1)
                BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
            if ((newFile = file + 1) <= 8)
                BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
        }
        pawnAttacks[B][square] = attacks;

        attacks = 0ULL;
        /* White pawn's attacks */
        if ((newRank = rank + 1) <= 8) {
            if ((newFile = file - 1) >= 1)
                BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
            if ((newFile = file + 1) <= 8)
                BitManipulation::setBit(&attacks, (newRank - 1) * 8 + (newFile - 1));
        }
        pawnAttacks[W][square] = attacks;
    }
}

void LookupTable::initPawnMoves() {
    memset(pawnMoves, 0, sizeof(pawnMoves));

    for (int square = 0; square < 64; square++) {
        u64 moves = 0ULL;

        int rank = (square / 8) + 1;
        int file = (square % 8) + 1;

        if (rank == 1 || rank == 8)
            continue;

        BitManipulation::setBit(&moves, rank * 8 + (file - 1));

        pawnMoves[W][square] = moves;
        moves = 0ULL;

        BitManipulation::setBit(&moves, (rank - 2) * 8 + (file - 1));
        
        pawnMoves[B][square] = moves;
    }
}

void LookupTable::initBitBetween() {
    memset(bitBetween, 0, sizeof(bitBetween));

    int srcRank, srcFile, desRank, desFile;
    for (int src = 0; src < 64; src++) {
        srcRank = src / 8;
        srcFile = src % 8;
        for (int des = 0; des < 64; des++) {
            if (src == des)
                continue;

            desRank = des / 8;
            desFile = des % 8;
            
            int startFile, startRank;
            if (srcRank == desRank && srcFile < desFile)
                for (startFile = srcFile + 1; startFile < desFile; startFile++)
                    BitManipulation::setBit(&bitBetween[src][des], srcRank * 8 + startFile);
            else if (srcRank == desRank && srcFile > desFile)
                for (startFile = srcFile - 1; startFile > desFile; startFile--)
                    BitManipulation::setBit(&bitBetween[src][des], srcRank * 8 + startFile);
            else if (srcRank < desRank && srcFile == desFile)
                for (startRank = srcRank + 1; startRank < desRank; startRank++)
                    BitManipulation::setBit(&bitBetween[src][des], startRank * 8 + srcFile);
            else if (srcRank > desRank && srcFile == desFile)
                for (startRank = srcRank - 1; startRank > desRank; startRank--)
                    BitManipulation::setBit(&bitBetween[src][des], startRank * 8 + srcFile);

            if (abs(srcRank - desRank) != abs(srcFile - desFile))
                continue;
            
            for (startRank = srcRank + 1, startFile = srcFile + 1; startRank < desRank && startFile < desFile; startRank++, startFile++)
                BitManipulation::setBit(&bitBetween[src][des], startRank * 8 + startFile);
            for (startRank = srcRank - 1, startFile = srcFile + 1; startRank > desRank && startFile < desFile; startRank--, startFile++)
                BitManipulation::setBit(&bitBetween[src][des], startRank * 8 + startFile);
            for (startRank = srcRank - 1, startFile = srcFile - 1; startRank > desRank && startFile > desFile; startRank--, startFile--)
                BitManipulation::setBit(&bitBetween[src][des], startRank * 8 + startFile);
            for (startRank = srcRank + 1, startFile = srcFile - 1; startRank < desRank && startFile > desFile; startRank++, startFile--)
                BitManipulation::setBit(&bitBetween[src][des], startRank * 8 + startFile);
        }
    }
}