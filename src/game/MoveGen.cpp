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

// void MoveGen::genMoves(BitBoard* bb, int piece, int square) {
//     u64 moves, captures, allies, enemies, pRank;
//     int rank, file;
    
//     allies = bb->allPositions[bb->side2play];
//     enemies = bb->allPositions[bb->next2play];
//     rank = square / 8;
//     file = square % 8;
//     pRank = (bb->side2play) ? 1 : 6;

//     switch (piece)
//     {
//     case PAWN:
//         bb->captures[square] = LookupTable::pawnAttacks[bb->side2play][square] & enemies & (~allies);
//         moves = LookupTable::pawnMoves[bb->side2play][square] & (~(allies | enemies));
//         if (rank == pRank && moves != 0ULL) // Mannualy check for double moves
//             if (bb->side2play)
//                 BitManipulation::setBit(&moves, (rank + 1) * 8 + (file - 1));
//             else
//                 BitManipulation::setBit(&moves, (rank - 3) * 8 + (file - 1));
                
//         bb->moves[square] = moves & (~(allies | enemies));     
//         break;
//     case ROOK:
        
//     /* codes */
//         break;
//     case KNIGHT:
//         bb->captures[square] = LookupTable::knightAttacks[square] & enemies;
//         bb->moves[square] = LookupTable::knightAttacks[square] & (~(allies | enemies));
//         break;
//     default:
//         return;
//         break;
//     }
// }