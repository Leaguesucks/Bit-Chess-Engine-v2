#include "../headers/Sliders.h"

namespace Sliders {
    u64 relevantBishopAttacks[64];
    u64 relevantRookAttacks[64];
    bool init = false;
}

void Sliders::initialize() {
    if (init)
        return;

    for (int square = 0; square < 64; square++) {
        relevantBishopAttacks[square] = getRelevantFreeBishopAttacks(square);
        relevantRookAttacks[square] = getRelevantFreeRookAttacks(square);
    }
    
    init = true;
}

u64 Sliders::getRelevantFreeBishopAttacks(int square) {
    u64 attacks = 0ULL;

    int curRk = square / 8; // Curren rank (0-7, top to bottom)
    int curFl = square % 8; // Current file (0-7, left to right)

    for (int rank = curRk - 1, file = curFl - 1; 
        rank >= 1 && file >= 1; rank--, file--) 
            BitManipulation::setBit(&attacks, file + 8*rank); // SW
    for (int rank = curRk - 1, file = curFl + 1; 
        rank >= 1 && file <= 6; rank--, file++) 
            BitManipulation::setBit(&attacks, file + 8*rank); // SE
    for (int rank = curRk + 1, file = curFl - 1; 
        rank <= 6 && file >= 1; rank++, file--) 
            BitManipulation::setBit(&attacks, file + 8*rank); // NW
    for (int rank = curRk + 1, file = curFl + 1; 
        rank <= 6 && file <= 6; rank++, file++) 
            BitManipulation::setBit(&attacks, file + 8*rank); // NE

    return attacks;
}

u64 Sliders::getRelevantFreeRookAttacks(int square) {
    u64 attacks = 0ULL;

    int curRk = square / 8; // Current rank (0-7, top to bottom)
    int curFl = square % 8; // Current file (0-7, left to right)

    for (int file = curFl - 1, decrement = 1; file >= 1; file--, decrement++) 
        BitManipulation::setBit(&attacks, square - decrement); // Left
    for (int file = curFl + 1, increment = 1; file <= 6; file++, increment++) 
        BitManipulation::setBit(&attacks, square + increment); // Right
    for (int rank = curRk - 1, decrement = 1; rank >= 1; rank--, decrement++) 
        BitManipulation::setBit(&attacks, square - 8*decrement); // Down
    for (int rank = curRk + 1, increment = 1; rank <= 6; rank++, increment++) 
        BitManipulation::setBit(&attacks, square + 8*increment); // Up

    return attacks;
}

u64 Sliders::getBlockedBishopAttacks(u64 blockers, int square) {
    u64 attacks = 0ULL;

    int curRk = square / 8; // Curren rank (0-7, top to bottom)
    int curFl = square % 8; // Current file (0-7, left to right)

    for (int rank = curRk - 1, file = curFl - 1; rank >= 0 && file >= 0; rank--, file--) { // SW
        BitManipulation::setBit(&attacks, file + 8*rank);
        if (BitManipulation::getBit(blockers, file + 8*rank) != 0) 
            break; // Blocker encountered 
    }
    for (int rank = curRk - 1, file = curFl + 1; rank >= 0 && file <= 7; rank--, file++) { // SE
        BitManipulation::setBit(&attacks, file + 8*rank);
        if (BitManipulation::getBit(blockers, file + 8*rank) != 0) 
            break; // Blocker encountered
    }
    for (int rank = curRk + 1, file = curFl - 1; rank <= 7 && file >= 0; rank++, file--) { // NW
        BitManipulation::setBit(&attacks, file + 8*rank);
        if (BitManipulation::getBit(blockers, file + 8*rank) != 0) 
            break; // Blocker encountered
    }
    for (int rank = curRk + 1, file = curFl + 1; rank <= 7 && file <= 7; rank++, file++) { // NE
        BitManipulation::setBit(&attacks, file + 8*rank);
        if (BitManipulation::getBit(blockers, file + 8*rank) != 0) 
            break; // Blocker encountered
    }

    return attacks;
}

u64 Sliders::getBlockedRookAttacks(u64 blockers, int square) {
    u64 attacks = 0ULL;

    int curRk = square / 8; // Current rank (0-7, top to bottom)
    int curFl = square % 8; // Current file (0-7, left to right)

    for (int file = curFl - 1, decrement = 1; file >= 0; file--, decrement++) {
        BitManipulation::setBit(&attacks, square - decrement);
        if (BitManipulation::getBit(blockers, square - decrement) != 0) 
            break; // Blocker encountered 
    }
    for (int file = curFl + 1, increment = 1; file <= 7; file++, increment++) { // Right
        BitManipulation::setBit(&attacks, square + increment);
        if (BitManipulation::getBit(blockers, square + increment) != 0) 
            break; // Blocker encountered 
    }
    for (int rank = curRk - 1, decrement = 1; rank >= 0; rank--, decrement++) { // Down
        BitManipulation::setBit(&attacks, square - 8*decrement); 
        if (BitManipulation::getBit(blockers, square - 8*decrement) != 0) 
            break; // Blocker encountered
    }
    for (int rank = curRk + 1, increment = 1; rank <= 7; rank++, increment++) { // Up
        BitManipulation::setBit(&attacks, square + 8*increment); 
        if (BitManipulation::getBit(blockers, square + 8*increment) != 0) 
            break; // Blocker encountered
    }

    return attacks;    
}

u64 Sliders::findRelevantSquares(u32 index, u64 rev_attacks) {
    u64 rev_squares = 0ULL;
    int numBits = BitManipulation::countBit(rev_attacks); // Number of relevant attacks

    for (int i = 0; i < numBits; i++) {
        int square = BitManipulation::getLSSB(rev_attacks);
        BitManipulation::popBit(&rev_attacks, square);
        if ((index & (1 << i)) != 0) 
            BitManipulation::setBit(&rev_squares, square);
    }

    return rev_squares;
}