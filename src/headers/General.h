#pragma once

#include <string>

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long

#define PAWN   0
#define KNIGHT 1
#define BISHOP 2
#define ROOK   3
#define QUEEN  4
#define KING   5

#define W 0
#define B 1

#define NORTH 0
#define NE 1
#define EAST 2
#define SE 3
#define SOUTH 4
#define SW 5
#define WEST 6
#define NW 7

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

const std::string squareStr[] = {
    "H1", "G1", "F1", "E1", "D1", "C1", "B1", "A1",
    "H2", "G2", "F2", "E2", "D2", "C2", "B2", "A2",
    "H3", "G3", "F3", "E3", "D3", "C3", "B3", "A3",
    "H4", "G4", "F4", "E4", "D4", "C4", "B4", "A4",
    "H5", "G5", "F5", "E5", "D5", "C5", "B5", "A5",
    "H6", "G6", "F6", "E6", "D6", "C6", "B6", "A6",
    "H7", "G7", "F7", "E7", "D7", "C7", "B7", "A7",
    "H8", "G8", "F8", "E8", "D8", "C8", "B8", "A8",
};