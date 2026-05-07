#pragma once

#include <string.h>

#include "General.h"
#include "Game.h"

/**
 * @brief Handle communication between the engine and the GUI
 */
class UCI {
    private:
        Game game;

    public:
        /**
         * @brief Create a new game starting from the beginning position
         */
        UCI();

        /**
         * @param game The reference to the game being played
         */
        UCI(Game& game);
};