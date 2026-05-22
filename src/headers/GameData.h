#pragma once

#include "Global.h"

#define NORMAL 0
#define CAPTURE 1
#define ENPASSEN 2
#define PROMOTION 3
#define CASTLE 4
#define PAWN_MOVE 5
#define PAWN_DOUBLE_MOVE 6

#define INVALID_MOVE 0
#define PENDING_PROMOTE_REQUEST 1
#define PENDING_PROMOTE_COMPLETE 2
#define SUCCESSFUL_MOVE 3

#define FIFTY_INITIAL 100

/**
 * @brief Contains some useful type definition for the game data
 */
namespace GameData {
   /*
    * @note The moves is encoded in a u64 number as followed:
    * ```
    *  bits 0: Which side to play
    *  bits 1-3: Which piece to move
    *  bits 4-9: From which square
    *  bits 10-15: To which square
    *  bits 16-18: What type of move:
    *      - 000: Normal move
    *      - 001: Capture
    *      - 010: Enpassen
    *      - 011: Promotion
    *      - 100: Castling (100 -> king side, 101 -> queen side)
    *      - 110: Pawn move
    *      - 111: Double pawn move
    * bits 19-21: What capture piece (0 if none)
    * bits 22-25: Castling flags BEFORE this move
    * bits 26-28: What promoted piece
    * bits 29-31: Redundant
    * ```
    */
    struct MoveInfo {
        int side; // Which side to move
        int piece; // Which piece to move
        int from; // The source square
        int to; // The destination square
        int type; // Which type of move
        int capture; // Which captured piece (if any)
        int promote; // Promote to what piece (if it is a promotion)
        u8 prevCastle; // The castling flag before this move
        bool invalid; // Set to true if this move is invalid
        int status; // If this move is in the middle of calculation. Used to skip some redundant calculation
        int castleSide; // King or Queen side castling
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

        int winner; // Negative number if not determined. 0 = White wins, 1 = Black wins, 2 = draw
    };
};