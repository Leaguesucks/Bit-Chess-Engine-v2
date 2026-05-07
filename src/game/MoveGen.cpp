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

void MoveGen::genMoves(Game& game, int piece, int square) {
    u64 moves, captures, allies, enemies, pRank, relevantBlockers;
    int rank, file;
    BitBoard* bb;
    u64 *pCaptures, *pMoves, *pCheckSources; 
    
    bb = game.getBoard();
    pCaptures = game.getCaptures();
    pMoves = game.getMoves();
    pCheckSources = game.getCheckSources();
    allies = bb->allPositions[bb->side2play];
    enemies = bb->allPositions[bb->side2play ^ 1];
    rank = square / 8;
    file = square % 8;
    pRank = (bb->side2play) ? 1 : 6;

    switch (piece)
    {
    case PAWN:
        pCaptures[square] = LookupTable::pawnAttacks[bb->side2play][square] & enemies & (~allies);
        moves = LookupTable::pawnMoves[bb->side2play][square] & (~(allies | enemies));
        if (rank == pRank && moves != 0ULL) // Mannualy check for double moves
            if (bb->side2play)
                BitManipulation::setBit(&moves, (rank + 1) * 8 + (file - 1));
            else
                BitManipulation::setBit(&moves, (rank - 3) * 8 + (file - 1));
                
        pMoves[square] = moves & (~(allies | enemies));     
        break;
    case ROOK:
        relevantBlockers = Sliders::relevantRookAttacks[square] & (allies | enemies);
        genRawRookBishopMove(game, false, square);
        break;
    case KNIGHT:
        pCaptures[square] = LookupTable::knightAttacks[square] & enemies;
        pMoves[square] = LookupTable::knightAttacks[square] & (~(allies | enemies));
        break;
    default:
        return;
        break;
    }
}

void MoveGen::genRawRookBishopMove(Game& game, bool bishop, int square) {
    u64 allies, enemies, revBlockers, index;
    BitBoard* bb;

    bb = game.getBoard();
    allies = bb->allPositions[bb->side2play];
    enemies = bb->allPositions[bb->side2play ^ 1];

    if (bishop) {
        revBlockers = Sliders::relevantBishopAttacks[square] & (allies | enemies);
        index = (revBlockers * LookupTable::BISHOP_MAGIC_NUMBERS[square]) >> (64 - Sliders::NUM_BISHOP_RELEVANT_SQUARES[square]);
        game.getMoves()[square] = LookupTable::bishopAttacks[square][index];
    } else {
        revBlockers = Sliders::relevantRookAttacks[square] & (allies | enemies);
        index = (revBlockers * LookupTable::ROOK_MAGIC_NUMBERS[square]) >> (64 - Sliders::NUM_ROOK_RELEVANT_SQUARES[square]);
        game.getMoves()[square] = LookupTable::rookAttacks[square][index];
    }
}