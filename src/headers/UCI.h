#pragma once

#include <sstream>
#include <string>
#include <iostream>

#include "Global.h"
#include "Game.h"
#include "GameData.h"

#define BUFFSIZE 100
#define ERROR_MSG "Invalid Move"

/**
 * @brief Handle communication between the engine and the GUI
 */
class UCI {
    private:
        Game* game;

    public:
        /**
         * @param game The pointer to the game being played
         */
        UCI(Game* game);

        /**
         * @brief Send a message to the GUI
         * @param msg The message to send
         */
        void send(std::string msg);

        /**
         * @return The messsage sent from the GUI
         * @note Return an std::string for safty
         */
        std::string recv();

        /**
         * @brief Connect to the GUI
         */
        void connect();

    private:
        /**
         * @brief Process the message from the GUI
         * @param msg The message sent from the GUI
         */
        void processMsg(std::string msg);

        /**
         * @brief Process the position request
         * @param msg The message sent from the GUI, which should be a position command
         * @return A response to the GUI
         */
        std::string processPosition(std::string msg);

        /**
         * @brief Move the pieces on the chess board if it is a move command
         * @param move The move field to process
         * @return A response to the GUI 
         * @note The move annotation sent from the GUI will be simplier to reduce the computation strain
         */
        std::string processMoves(std::string move);

        /**
         * @brief Process the move sent from the GUI, which is simplier than a standard Algebraic Notation
         * @param move The move field to process
         * @return A response to the GUI
         */
        std::string processSimpleMoves(std::string move);

        /**
         * @brief Build a move information struct
         * @param move The string that represent the move being made
         * @param simple False if the encoded move is from-to styled, true other wise (e.g., Qh4 style)
         * @return A struct that contains all needed information about the move
         * @see resources/UCI/CommProtocol.txt for more information
         */
        GameData::MoveInfo buildMove(std::string move);

};