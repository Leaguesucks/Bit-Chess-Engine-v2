#include "../headers/MoveGen.h"

namespace MoveGen {
    bool init = false;
}

void MoveGen::initialize() {
    if (init)
        return;

    LookupTable::initialize();
    init = true;
}

void MoveGen::genMoves(BitBoard& bb, CalBoard& cb, int piece, int square) {
    u64 moves, captures, allies, enemies, relevantBlockers;
    int rank, file, pRank;
    
    allies = bb.allPositions[bb.side2play];
    enemies = bb.allPositions[bb.side2play ^ 1];
    rank = square / 8;
    file = square % 8;
    pRank = (bb.side2play) ? 1 : 6;

    switch (piece)
    {
    case PAWN:
        cb.captures[square] = LookupTable::pawnAttacks[bb.side2play][square] & enemies & (~allies);
        moves = LookupTable::pawnMoves[bb.side2play][square] & (~(allies | enemies));
        if (rank == pRank && moves != 0ULL) // Mannualy check for double moves
            if (bb.side2play)
                BitManipulation::setBit(&moves, (rank + 1) * 8 + (file - 1));
            else
                BitManipulation::setBit(&moves, (rank - 3) * 8 + (file - 1));
                
        cb.moves[square] = moves & (~(allies | enemies));     
        break;
    case ROOK:
        relevantBlockers = Sliders::relevantRookAttacks[square] & (allies | enemies);
        genRawRookBishopMove(bb, cb, false, square);
        break;
    case BISHOP:
        relevantBlockers = Sliders::relevantBishopAttacks[square] & (allies | enemies);
        genRawRookBishopMove(bb, cb, true, square);
        break;
    case KNIGHT:
        cb.captures[square] = LookupTable::knightAttacks[square] & enemies;
        cb.moves[square]= LookupTable::knightAttacks[square] & (~(allies | enemies));
        // std::cerr << squareStr[square] << std::endl;
        // printBoard(LookupTable::knightAttacks[square], std::cerr);
        // printBoard(LookupTable::knightAttacks[square] & ~ (allies | enemies), std::cerr);
        // printBoard(cb.moves[square], std::cerr);
        // printBoard(allies, std::cerr);
        // printBoard(enemies, std::cerr);
        break;
    default:
        return;
        break;
    }
}

void MoveGen::genRawRookBishopMove(BitBoard& bb, CalBoard& cb, bool bishop, int square) {
    u64 allies, enemies, revBlockers, index;

    allies = bb.allPositions[bb.side2play];
    enemies = bb.allPositions[bb.side2play ^ 1];

    if (bishop) {
        revBlockers = Sliders::relevantBishopAttacks[square] & (allies | enemies);
        index = (revBlockers * LookupTable::BISHOP_MAGIC_NUMBERS[square]) >> (64 - Sliders::NUM_BISHOP_RELEVANT_SQUARES[square]);
        cb.moves[square] = LookupTable::bishopAttacks[square][index];
    } else {
        revBlockers = Sliders::relevantRookAttacks[square] & (allies | enemies);
        index = (revBlockers * LookupTable::ROOK_MAGIC_NUMBERS[square]) >> (64 - Sliders::NUM_ROOK_RELEVANT_SQUARES[square]);
        cb.moves[square] = LookupTable::rookAttacks[square][index];
    }
}