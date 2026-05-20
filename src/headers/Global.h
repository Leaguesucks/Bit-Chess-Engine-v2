#pragma once

#include <string>
#include <cstdint>

#include "../../debug/debug.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define White 0
#define Black 1

#define KING_SIDE 0
#define QUEEN_SIDE 1

#define NORTH 0
#define NE 1
#define EAST 2
#define SE 3
#define SOUTH 4
#define SW 5
#define WEST 6
#define NW 7

typedef enum Piece {
    PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING, EMPTY
} Piece;

/**
 * @note H1 is the LSB while A8 is the MSB
 */
typedef enum Square {
    H1, G1, F1, E1, D1, C1, B1, A1,
    H2, G2, F2, E2, D2, C2, B2, A2,
    H3, G3, F3, E3, D3, C3, B3, A3,
    H4, G4, F4, E4, D4, C4, B4, A4,
    H5, G5, F5, E5, D5, C5, B5, A5,
    H6, G6, F6, E6, D6, C6, B6, A6,
    H7, G7, F7, E7, D7, C7, B7, A7,
    H8, G8, F8, E8, D8, C8, B8, A8,
} Square;

const std::string squareStr[64] = {
    "H1", "G1", "F1", "E1", "D1", "C1", "B1", "A1",
    "H2", "G2", "F2", "E2", "D2", "C2", "B2", "A2",
    "H3", "G3", "F3", "E3", "D3", "C3", "B3", "A3",
    "H4", "G4", "F4", "E4", "D4", "C4", "B4", "A4",
    "H5", "G5", "F5", "E5", "D5", "C5", "B5", "A5",
    "H6", "G6", "F6", "E6", "D6", "C6", "B6", "A6",
    "H7", "G7", "F7", "E7", "D7", "C7", "B7", "A7",
    "H8", "G8", "F8", "E8", "D8", "C8", "B8", "A8",
};

/**
 * @brief Holds the general information of the game, such as the pieces' positions
 */
struct BitBoard {
    u64 positions[2][6]; // Positions of each piece on each side
    u64 allPositions[2]; // Positions of all piece in a single bit map on each side
    u8 board[64]; // The board with each piece's character value on it. Used for fast communication with the GUI
    u32 fly; // The total number of full move that has been played
    u8 side2play; // The side to play

    /** The castling right for both side
     * 0001 = white king side
     * 0010 = white queen side
     * 0100 = black king side
     * 1000 = black queen side
     */
    u8 castlingRights;
    u8 fifty; // Fifty move rule, decrement everytime a ply is played. Reset to 100 after every pawn move or capture
    int enPassen; // The enPassen square, which is the square behind the victim
};

/**
 * @brief Holds the information for the engine calculation, such as moves, attacks, etc
 */
struct CalBoard {
    // All possible move for each square. Reset at the begining of each ply
    // A move can include captures, except for pawn
    u64 moves[64]; 

    u64 captures[64]; // All possible captures for each square. Reset at the begining of each ply
    u64 checkSources[2]; // Stores the sources of check for each side
    u64 pinned; // Masks of pinned pieces
    u64 allPossibleAttacks[2]; // Mask of all POSSIBLE/POTENTIAL attacks (moves + pawn's captures) for each side
    u64 allLegalMoves[2]; // All LEGAL moves for each side
    int pinSources[64]; // Store the square of the pin source for each square

    // Protected positions. The king cannot capture a protected position/piece
    // Clear at the beginning of each moves generation / the end of each ply
    u64 procPositions[2];

    // Raise if it is possible for the king to castle. The bit encoding is identical to
    // the castlingRight flag in BitBoard
    u8 castlingFlags;
};