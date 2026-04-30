#include "../src/headers/General.h"

#include <iostream>

#define COLOR(code) "\033[" code "m"
#define BLK     "30"
#define RED     "31"
#define GREEN   "32"
#define YELLOW  "33"
#define BLUE    "34"
#define MAGENTA "35"
#define CYAN    "36"
#define WH      "37"
#define RESET   "0"

/**
 * @brief Print a bit board to stdout
 * 
 * Print an u64 number in a chess board representation to stdout
 * 
 * @param b The bit board to be printed
 * @note Square A8 is MSB and H1 is LSB
 */
void printBoard(u64 b);


