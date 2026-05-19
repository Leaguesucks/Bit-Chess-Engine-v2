#include "debug.h"

void printBoard(uint64_t b, std::ostream& out) {
    out << COLOR(GREEN)"   A B C D E F G H  \n\n" COLOR(RESET);

    for (int row = 0; row < 8; row++) {
        out << COLOR(GREEN) << 8 - row << "  " << COLOR(RESET);

        for (int col = 0; col < 8; col++) {
            int acol = 7 - col;
            int arow = 7 - row;
            if ((b >> (acol + arow*8)) & 1ULL)
                out << COLOR(YELLOW)"1 " COLOR(RESET);
            else
                out << "0 ";
        }
        out << COLOR(GREEN) "  " << 8 - row << " \n" COLOR(RESET);
    }

    out << COLOR(GREEN) "\n   A B C D E F G H  \n\n" COLOR(RESET);
}