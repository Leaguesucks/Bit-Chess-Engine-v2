#pragma once

#include <string>
#include <vector>

#include "General.h"

/**
 * @brief Holds the current information of the game, such as the pieces' position, moves
 *        and attacks, etc.
 */
struct BitBoard {
    u64 positions[2][6]; // Positions of each piece on each side
    u64 allPositions[2]; // Positions of all piece in a single bit map on each side
    u8 board[64]; // The board with each piece value on it. Used for fast communication with the GUI

    u32 ply; // The total number of one-half move that has been played
    u8 fifty; // Fifty move rule, decrement everytime a ply is played. Reset to 100 after every pawn move or capture
    u8 side2play; // The side to play
    u8 enemy2play; // The next (enemy) side to play, which is the inverse of the side to play

    /** The castling right for both side
     * 0001 = white king side
     * 0010 = white queen side
     * 0100 = black king side
     * 1000 = black queen side
     */
    u8 castlingRights;

    int enPassenSquare; // -1 if none
};


/** 
 * @brief Holds the current state of the game, such as the total number of moves, who
 *        to play, etc.
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
 *      - 100: Castling
 *      - 101: Pawn move
 * bits 19-21: What captured piece (0 if none)
 * bits 22-24: Promote to what piece (0 if none)
 * bits 25-28: Castling flags:
 *      - 0001: Remove white's king castling right
 *      - 0010: Remove white's queen castling right
 *      - 0100: Remove black's king castling right
 *      - 1000: Remove black's queen castling right
 * bits 29-34: Any Enpassen square that this move "turn off". 0 if none
 * bits 35-63: Redundant
 *      
 * ```
 */
class Game {
    private:
        struct BitBoard board; // Holds the current information of the game
        std::vector<u64> moveStacks; // Store the history of moves
        u64 moves[64]; // All possible move for each square. Reset at the begining of each ply
        u64 captures[64]; // All possible captures for each square. Reset at the begining of each ply
        u64 checkSources[2]; // Stores the sources of check for each side

    public:
        /**
         * @brief Default constructor: Load a new game with the default position
         */
        Game();

        /**
         * @brief Load a new game from a FEN
         */
        Game(const std::string fen);

        /**
         * @brief Additionally, can also use this method to start a new game from a FEN
         */
        void setFEN(const std::string fen);

        /* Setters and getters. For performance we return the pointers */

        u64* getMoves();
        u64* getCaptures();
        u64* getCheckSources();
        BitBoard* getBitBoard();

        /**
         * @brief Move a piece
         * 
         * @param move The encoded move
         */
        void makeMove(u64 move);

        /**
         * @brief Undo a move
         */
        void undo();
};