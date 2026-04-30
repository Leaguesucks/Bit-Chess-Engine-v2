#include "debug.h"

void printBoard(u64 b) {
    std::cout << COLOR(GREEN)"   A B C D E F G H  \n\n" COLOR(RESET);

    for (int row = 0; row < 8; row++) {
        std::cout << COLOR(GREEN) << 8 - row << "  " << COLOR(RESET);

        for (int col = 0; col < 8; col++) {
            int acol = 7 - col;
            int arow = 7 - row;
            if ((b >> (acol + arow*8)) & 1ULL)
                std::cout << COLOR(YELLOW)"1 " COLOR(RESET);
            else
                std::cout << "0 ";
        }
        std::cout << COLOR(GREEN) "  " << 8 - row << " \n" COLOR(RESET);
    }

    std::cout << COLOR(GREEN) "\n   A B C D E F G H  \n\n" COLOR(RESET);
}