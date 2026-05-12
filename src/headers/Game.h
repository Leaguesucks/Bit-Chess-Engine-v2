#pragma once

#include <cstring>
#include <string.h>
#include <vector>

#include "General.h"
#include "BitManipulation.h"

#define BEGIN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

/**
 * @brief Holds the general information of the game, such as the pieces' positions
 */
struct BitBoard {
    u64 positions[2][6]; // Positions of each piece on each side
    u64 allPositions[2]; // Positions of all piece in a single bit map on each side
    u8 board[64]; // The board with each piece value on it. Used for fast communication with the GUI
    u32 fly; // The total number of full move that has been played
    u8 side2play; // The side to play

    /** The castling right for both side
     * 0001 = white king side
     * 0010 = white queen side
     * 0100 = black king side
     * 1000 = black queen side
     */
    u8 castlingRights;
};

/**
 * @brief Holds the information for the engine calculation, such as moves, attacks, etc
 */
struct CalBoard {
    u64 moves[64]; // All possible move for each square. Reset at the begining of each ply
    u64 captures[64]; // All possible captures for each square. Reset at the begining of each ply
    u64 checkSources; // Stores the sources of check for each side
    u64 pinned; // Masks of pinned pieces
    u64 enPassen; // Masks of the enpassen squares, which is the square behind the victim pawn
    u8 fifty; // Fifty move rule, decrement everytime a ply is played. Reset to 100 after every pawn move or capture
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
 *      - 100: Castling (100 -> king side, 101 -> queen side)
 *      - 110: Pawn move
 *      - 111: Double pawn move
 * bits 19-21: What captured/promoted piece (0 if none)
 * bits 22-25: Castling flags BEFORE this move
 * bits 26-31: Redundant
 * ```
 */
class Game {
    private:
        BitBoard board; // Holds the current information of the game
        CalBoard calBoard; // Holds the information for the engine calculation
        std::vector<u32> moveStacks; // Store the history of moves

    public:
        /**
         * @brief Load a new game from a FEN
         */
        Game(char* fen);

        /**
         * @brief Additionally, can also use this method to start a new game from a FEN
         */
        void setFEN(char* fen);

        /* Setters and getters. For performance we return the references */

        CalBoard* getCalBoard();
        BitBoard* getBoard();

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

        /**
         * @param square The square to calculate the moves
         * @return The moves from a square. Used to communicate with the GUI
         */
        u64 getMovesOnSquare(int square);

        /**
         * @param square The square to calculate the captures
         * @return The captures from a square. Used to communicate with the GUI
         */
        u64 getCapturesOnSquare(int square);

    private:
        /**
         * @brief Default constructor
         */
        Game();

        /**
         * @brief Set up the piece position from a FEN field
         * 
         * @param token The field of the FEN that describes how to set up the pieces
         */
        void setPosition(char* token);

        /**
         * @brief Set the castle right from a FEN field
         * 
         * @param token The field of the FEN that describes the castle right
         */
        void setCastleRight(char* token);

        /**
         * @brief Masks the enpassen squares from a FEN field
         * 
         * @param token The field of the FEN that describes the enpassen squares
         */
        void setEnPassen(char* token);
};