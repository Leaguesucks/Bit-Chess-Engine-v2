#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

#include "../debug/debug.h"
#include "headers/General.h"
#include "headers/BitManipulation.h"
#include "headers/Sliders.h"
#include "headers/Magic.h"
#include "headers/LookupTable.h"
#include "headers/Game.h"
#include "headers/UCI.h"

int main() {
    std::cout.setf(std::ios::unitbuf);

    Game game(BEGIN);
    UCI uci(&game);

    uci.connect();

    return 0;
}
