#include <iostream>

#include "../debug/debug.h"
#include "headers/General.h"
#include "headers/BitManipulation.h"
#include "headers/Sliders.h"
#include "headers/Magic.h"
#include "headers/LookupTable.h"
#include "headers/Game.h"

int main() {
    LookupTable::initialize();
    
    // for (int i = 0; i < 64; i++) {
    //     for (int j = 0; j < 64; j++) {
    //         std::cout << "Between square " << squareStr[i] << " and " << squareStr[j] << std::endl;
    //         printBoard(LookupTable::bitBetween[i][j]);
    //         getchar();
    //     }
    // }

    printBoard(LookupTable::bitBetween[A1][H8]);

    return 0;
}
