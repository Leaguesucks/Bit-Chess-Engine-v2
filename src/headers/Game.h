#pragma once

#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

#include "General.h"
#include "BitManipulation.h"
#include "MoveGen.h"

#define BEGIN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

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
        Game(std::string fen);

        /**
         * @brief Additionally, can also use this method to start a new game from a FEN
         */
        void setFEN(std::string fen);

        /**
         * @brief Convert the current state of the game to a fen string
         * @return A FEN string that represents the current game being played
         */
        std::string toFEN();

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

    private:
        /**
         * @brief Set up the piece position from a FEN field
         * 
         * @param token The field of the FEN that describes how to set up the pieces
         */
        void setPosition(std::string token);

        /**
         * @brief Set the castle right from a FEN field
         * 
         * @param token The field of the FEN that describes the castle right
         */
        void setCastleRight(std::string token);

        /**
         * @brief Masks the enpassen squares from a FEN field
         * 
         * @param token The field of the FEN that describes the enpassen squares
         */
        void setEnPassen(std::string token);

        /**
         * @brief Calculate moves for the side to play
         */
        void calculateMoves();

        /**
         * @brief Clean up i.e., set all members to zero
         */
        void reset();
};