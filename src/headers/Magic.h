#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>

#include "General.h"
#include "BitManipulation.h"
#include "Sliders.h"

/**
 * @brief Responsible for magic creation tasks
 * 
 * This namespace handles creating magic number that can be used for quick search through the
 * lookup table for slider pieces
 */ 
namespace Magic {
    /**
     * @brief Print the magic number for the rook and bishop
     * 
     * Use this function ONCE or as many times as you like to print the magic numbers that
     * can be used to lookup rook and bishop's moves
     * 
     * @note Copy & paste the result into the magic numbers tables
     */
    void printMagic();
}