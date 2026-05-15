#pragma once

#include <string.h>
#include <string>
#include <iostream>

#include "General.h"
#include "Game.h"

#define BUFFSIZE 100

/**
 * @brief Handle communication between the engine and the GUI
 * @todo Get rid of the use of C-like string and switch to std::string for safety
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
        void send(const char* msg);

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
        void processMsg(const char* msg);

        /**
         * @brief Process the position request
         * @param msg The message sent from the GUI, which should be a position command
         */
        void processPosition(const char* msg);
};