#pragma once

#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

#include "Global.h"
#include "GameData.h"
#include "BitManipulation.h"
#include "MoveGen.h"
#include "LookupTable.h"

#define BEGIN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

/** 
 * @brief Holds the current state of the game, such as the total number of moves, who
 *        to play, etc.
 */
class Game {
    private:
        GameData::BitBoard board; // Holds the current information of the game
        GameData::CalBoard calBoard; // Holds the information for the engine calculation
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

        GameData::CalBoard* getCalBoard();
        GameData::BitBoard* getBoard();

        /**
         * @brief Move a piece
         * @param moveInfo Contains the source and destination square and promotion (if applied)
         * @return The move status: ERROR, or SUCESSFUL MOVE
         * @note We assume that most of the legal move checking has been done by the GUI
         */
        int makeMove(GameData::MoveInfo moveInfo);

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
         * @brief Set the enpassen squares from a FEN field
         * 
         * @param token The field of the FEN that describes the enpassen squares
         */
        void setEnPassen(std::string token);

        /**
         * @brief Set the enpassen square after a double pawn move
         * @param side The color of the moved pawn
         * @param des The destination of the pawn
         */
        void setEnPassen(int side, int des);

        /**
         * @brief Masks the enpassen square after a pawn double move
         * 
         * @param des The destination after the enpassen move
         * @param side The color of the pawn being enpassen
         * @return The square of the victim pawn
         */
        int getEnPassenVictim(int des, int side);

        /**
         * @brief Calculate moves for the side to play
         * @param side The side to calculate the moves for
         */
        void calculateMoves(int side);

        /**
         * @brief Clean up i.e., set all members to zero
         */
        void reset();

        /**
         * @brief Make the pawn move
         * @param pMove Reference to the move struct to retrieve and update the move information
         */
        void movePawn(GameData::MoveInfo& pMove);

        /**
         * @brief Make the king move
        *  @param pMove Reference to the move struct to retrieve and update the move information
         */
        void moveKing(GameData::MoveInfo& pMove);

        /**
         * @brief Make the rook move
         * @param pMove Reference to the move struct to retrieve and update the move information
         */
        void moveRook(GameData::MoveInfo& pMove);

        /**
         * @brief Encode a move and push it onto the move stack
         * @param pMove The move struct to retrieve and update the move information
         */
        void encodeMove(const GameData::MoveInfo pMove);

        /**
         * @brief Handle updating after successful move
         * @param moveInfo Contains details about the move
         */
        void postSuccessfulMove(const GameData::MoveInfo moveInfo);

        /**
         * @brief Check what type of move it is. If it is a capture move then return the captured piece as well
         * @param pMove Reference to the move information to retrieve and update
         */
        void checkValidMove(GameData::MoveInfo& pMove);
};