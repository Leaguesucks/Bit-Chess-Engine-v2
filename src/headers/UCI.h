#pragma once

#include <string.h>
#include <string>
#include <stdio.h>
#include <iostream>

#include "General.h"
#include "Game.h"

#define BUFFSIZE 100

/**
 * @brief Handle communication between the engine and the GUI
 */
class UCI {
    private:
        Game* game;

    public:
        /**
         * @brief Default constructor
         */
        UCI();

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
        void processMsg(char* msg);
};