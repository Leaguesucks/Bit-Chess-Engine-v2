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

void MoveGen::genMoves(GameData::BitBoard& bb, GameData::CalBoard& cb, int piece, int square, int side) {
    u64 rawAttacks, allies, enemies, bitSq, pinSrcMask, enPassenMask;
    int pinSrcSq;
    maskReturn moveMasks;

    cb.moves[square] = 0ULL;
    cb.captures[square] = 0ULL;
    
    allies = bb.allPositions[side];
    enemies = bb.allPositions[side ^ 1];

    switch (piece)
    {
    case PAWN:
        rawAttacks = genPawnMove(bb, cb, square, side);
        break;
    case ROOK:
        moveMasks = genRookBishopMove(bb, false, true, square, side);
        cb.moves[square] = moveMasks.moves; 
        cb.captures[square] = moveMasks.captures;
        rawAttacks = moveMasks.rawMoves;
        break;
    case BISHOP:
        moveMasks = genRookBishopMove(bb, true, true, square, side);
        cb.moves[square] = moveMasks.moves; 
        cb.captures[square] = moveMasks.captures;
        rawAttacks = moveMasks.rawMoves;
        break;
    case QUEEN:
        moveMasks = genRookBishopMove(bb, false, true, square, side);
        cb.moves[square] = moveMasks.moves;
        cb.captures[square] = moveMasks.captures;
        rawAttacks = moveMasks.rawMoves;
        moveMasks = genRookBishopMove(bb, true, true, square, side);
        cb.moves[square] |= moveMasks.moves;
        cb.captures[square] |= moveMasks.captures;
        rawAttacks |= moveMasks.rawMoves;
        break;
    case KNIGHT:
        rawAttacks = LookupTable::knightAttacks[square];
        cb.captures[square] = rawAttacks & enemies;
        cb.moves[square] = rawAttacks & ~allies;
        break;
    case KING:
        genKingMove(bb, cb, square, side);
        break;
    default:
        std::cerr << "This piece should not exist" << std::endl;
        break;
    }

    bitSq = 0ULL;
    BitManipulation::setBit(&bitSq, square);
    if (cb.pinned[side] & bitSq) {
        pinSrcSq = cb.pinSources[square];
        pinSrcMask = 0ULL;
        BitManipulation::setBit(&pinSrcMask, pinSrcSq);
        cb.moves[square] &= (LookupTable::bitBetween[square][pinSrcSq] | pinSrcMask);
        cb.captures[square] &= pinSrcMask;

        return; // No other moves allowed
    }

    handleCheck(bb, cb, square, piece, side);

    // @note Don't forget to re-calculate checksource after each move
    if (bb.positions[side ^ 1][KING] & cb.captures[square]) // Add the check source
        BitManipulation::setBit(&cb.checkSources[side], square);
    cb.allPossibleAttacks[side] |= rawAttacks & ~allies;
    cb.procPositions[side] |= (rawAttacks & allies);
    cb.allLegalMoves[side] |= cb.moves[square];
}

u64 MoveGen::genPawnMove(GameData::BitBoard& bb, GameData::CalBoard& cb, int square, int side) {
    u64 rawAttacks, moves, allies, enemies, enPassenMask;
    int rank, file, pRank;

    allies = bb.allPositions[side];
    enemies = bb.allPositions[side ^ 1];

    rank = square / 8;
    file = square % 8;
    pRank = (side == White) ? 1 : 6;

    enPassenMask = 0ULL;
    if (bb.enPassen > 0)
        BitManipulation::setBit(&enPassenMask, bb.enPassen);

    rawAttacks = LookupTable::pawnAttacks[side][square];
    cb.captures[square] = rawAttacks & enemies & (~allies);
    if (enPassenMask && cb.captures[square] & enPassenMask)
        cb.captures[square] |= enPassenMask;
    moves = LookupTable::pawnMoves[side][square] & (~(allies | enemies));
    if (rank == pRank && moves != 0ULL) // Mannualy check for double moves
        if (side == White)
            BitManipulation::setBit(&moves, (rank + 2) * 8 + file);
        else
            BitManipulation::setBit(&moves, (rank - 2) * 8 + file);
            
    moves &= ~(allies | enemies);
    cb.moves[square] = moves;
    
    return rawAttacks;
}

MoveGen::maskReturn MoveGen::genRookBishopMove(GameData::BitBoard& bb, bool bishop, bool blocker, int square, int side) {
    u64 allies, enemies, blockers, revBlockers, index;
    maskReturn sliderMasks;

    allies = bb.allPositions[side];
    enemies = bb.allPositions[side ^ 1];
    blockers = (blocker) ? (allies | enemies) : 0ULL;

    if (bishop) {
        revBlockers = Sliders::relevantBishopAttacks[square] & blockers;
        index = (revBlockers * LookupTable::BISHOP_MAGIC_NUMBERS[square]) >> (64 - Sliders::NUM_BISHOP_RELEVANT_SQUARES[square]);
        sliderMasks.rawMoves = LookupTable::bishopAttacks[square][index];
    } else {
        revBlockers = Sliders::relevantRookAttacks[square] & blockers;
        index = (revBlockers * LookupTable::ROOK_MAGIC_NUMBERS[square]) >> (64 - Sliders::NUM_ROOK_RELEVANT_SQUARES[square]);
        sliderMasks.rawMoves = LookupTable::rookAttacks[square][index];
    }

    sliderMasks.captures = sliderMasks.rawMoves & enemies;
    sliderMasks.moves = sliderMasks.rawMoves & ~allies | sliderMasks.captures;
    return sliderMasks;
}

void MoveGen::setPin(GameData::BitBoard& bb, GameData::CalBoard& cb, int side) {
    // Only the rook and bishop (and the queen - a hybrid) can pin other pieces
    u64 allies, enemies, rookPinners, bishopPinners, enemyRooks, enemyQueens, enemyBishops;
    int kingSq;
    maskReturn sliderMasks;
    
    allies = bb.allPositions[side];
    enemies = bb.allPositions[side ^ 1];
    enemyRooks = bb.positions[side ^ 1][ROOK];
    enemyBishops = bb.positions[side ^ 1][BISHOP];
    enemyQueens = bb.positions[side ^ 1][QUEEN];
    kingSq = BitManipulation::getLSSB(bb.positions[side][KING]);

    // Mask the rook attack rays from the king square
    sliderMasks = genRookBishopMove(bb, false, false, kingSq, side);
    rookPinners = sliderMasks.rawMoves & (enemyRooks | enemyQueens);

    // Mask the bishop attack rays from the king square
    sliderMasks = genRookBishopMove(bb, true, false, kingSq, side);
    bishopPinners = sliderMasks.rawMoves & (enemyBishops | enemyQueens);

    // Clean up and initialize
    cb.moves[kingSq] = 0ULL;
    cb.captures[kingSq] = 0ULL;
    cb.pinned[side] = 0ULL;

    setRookBishopPin(cb, kingSq, allies, rookPinners, side);
    setRookBishopPin(cb, kingSq, allies, bishopPinners, side);
}

void MoveGen::setRookBishopPin(GameData::CalBoard& cb, int kingSq, u64 allies, u64 pinners, int side) {
    int square;
    u64 allyBlockers;
    
    while (pinners) {
        square = BitManipulation::getLSSB(pinners);
        BitManipulation::popBit(&pinners, square);

        allyBlockers = LookupTable::bitBetween[square][kingSq] & allies;

        // Essentially if there is only one ally blocking the ray between the king and
        // the enemy sliders, then that ally is pinned.
        // Find out more on cool bit tricks here: https://www.geeksforgeeks.org/dsa/bit-tricks-competitive-programming/
        if (allyBlockers && !(allyBlockers & (allyBlockers - 1))) {
            cb.pinned[side] |= allyBlockers;
            cb.pinSources[BitManipulation::getLSSB(allyBlockers)] = square;
        }
    }
}

u64 MoveGen::genKingMove(GameData::BitBoard& bb, GameData::CalBoard& cb, int square, int side) {
    u64 allies, enemies, enemyAttacks, occupancy, rawMoves, betweenKing, betweenQueen;
    int rookKingStart, rookQueenStart, kingEnd;

    allies = bb.allPositions[side];
    enemies = bb.allPositions[side ^ 1];
    occupancy = allies | enemies;
    enemyAttacks = cb.allPossibleAttacks[side ^ 1];
    rookKingStart = LookupTable::ROOK_CASTLE_START_SQUARE[side][KING_SIDE];
    rookQueenStart = LookupTable::ROOK_CASTLE_START_SQUARE[side][QUEEN_SIDE];
    kingEnd = LookupTable::KING_CASTLE_START_SQUARE[side];
    betweenKing = LookupTable::bitBetween[square][rookKingStart];
    betweenQueen = LookupTable::bitBetween[square][rookQueenStart - 1];

    rawMoves = LookupTable::kingAttacks[square];
    cb.moves[square] = rawMoves & ~(allies | enemyAttacks | cb.procPositions[side ^ 1]);
    cb.captures[square] = cb.moves[square] & enemies & ~cb.procPositions[side ^ 1];

    // Check for castling. Cannot castle if:
    // - The castleRight masks is not set
    // - Is undercheck
    if (square != kingEnd || cb.checkSources[side ^ 1])
        return rawMoves;

    if (!(betweenKing & enemyAttacks) && !(betweenKing & occupancy)
        && (BitManipulation::getBit((u64) bb.castlingRights, side*2 + KING_SIDE)))
        BitManipulation::setBit(&cb.moves[square], LookupTable::KING_CASTLE_END_SQUARE[side][KING_SIDE]);

    
    if (!(betweenQueen & enemyAttacks) && !(betweenQueen & occupancy)
        && (BitManipulation::getBit((u64) bb.castlingRights, side*2 + QUEEN_SIDE)))
        BitManipulation::setBit(&cb.moves[square], LookupTable::KING_CASTLE_END_SQUARE[side][QUEEN_SIDE]);

    return rawMoves;
}

void MoveGen::handleCheck(GameData::BitBoard& bb, GameData::CalBoard& cb, int square, int piece, int side) {
    u8 checkPiece;
    int checkSrcSq;
    u64 checkSrc = cb.checkSources[side ^ 1];

    if (!checkSrc)
        return;

    if (BitManipulation::countBit(checkSrc) > 1 && piece != KING) {
        // Can't really do anything if theres is more than 1 check sources except
        // moving the king out of the check
        cb.moves[square] = 0ULL;
        cb.captures[square] = 0ULL;
        return;
    }

    cb.captures[square] &= checkSrc;
    cb.moves[square] &= checkSrc;

    checkSrcSq = BitManipulation::getLSSB(checkSrc);
    checkPiece = (side == White) ? tolower(bb.board[checkSrcSq]) : bb.board[checkSrcSq];

    if (checkPiece == 'r' || checkPiece == 'b' || checkPiece == 'q')
        cb.moves[square] &= LookupTable::bitBetween[square][checkSrcSq];
    else if (checkPiece == 'p') // Rarely an enPassen can coincidentally block a check
        cb.captures[square] &= LookupTable::bitBetween[square][checkSrcSq];
}