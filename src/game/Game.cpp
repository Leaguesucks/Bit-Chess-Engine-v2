#include "../headers/Game.h"

Game::Game(std::string fen) {
    setFEN(fen);
}

void Game::setFEN(std::string fen) {
    reset();

    std::stringstream ss(fen);
    std::string token;
    int field = 0;

    memset(board.board, EMPTY, sizeof(board.board));
    board.enPassen = -1;
    board.castlingRights = 0;
    while (ss >> token) {
        switch (field)
        {
        case 0:
            setPosition(token);
            break;
        case 1:
            board.side2play = (token[0] == 'w') ? White : Black;
            break;
        case 2:
            setCastleRight(token);
            break;
        case 3:
            setEnPassen(token);
            break;
        case 4:
            board.fifty = 100 - std::stoi(token);
            break;
        case 5:
            board.fly = std::stoi(token);
            break;
        default:
            std::cerr << "Invalid number of fields for a FEN" << std::endl;
            break;
        }
        field++;
    }
    calculateMoves();
}

std::string Game::toFEN() {
    std::string fen = "";
    int row, col, empty;
    const char castleSeq[] = {'K', 'Q', 'k', 'q'};

    for (row = 7; row >= 0; row--) { // position field
        empty = 0;
        for (col = 7; col >= 0; col--) {
            if (board.board[row*8 + col] == EMPTY) {
                empty++;
            } else {
                if (empty > 0) fen.append(std::to_string(empty));
                fen.append(1, board.board[row*8 + col]);
                empty = 0;                    
            }

            if (col == 0) 
                fen.append(std::to_string(empty));
        }
        (row > 0) ? fen.append("/") : fen.append(" ");
    }

    (board.side2play == White) ? fen.append("w ") : fen.append("b ");

    for (int c = 0; c < 4; c++) { // Castle right
        if (board.castlingRights == 0) {
            fen.append("-");
            break;
        }   
        if ((board.castlingRights >> c) & 1)
            fen.append(1, castleSeq[c]);
    }
    fen.append(" ");

    (board.enPassen < 0) ? fen.append("-") : fen.append(squareStr[board.enPassen]);
    fen.append(" ");

    fen.append(std::to_string(100 - board.fifty));
    fen.append(" ");

    fen.append(std::to_string(board.fly));

    return fen;
}

CalBoard* Game::getCalBoard() {
    return &calBoard;
}

BitBoard* Game::getBoard() {
    return &board;
}

void Game::makeMove(u64 move) {

}

void Game::undo() {

}

void Game::setPosition(std::string token) {
    char piece;
    int row, col, color;
    std::string rank;
    std::stringstream ss(token);
    const char P[6] = {'p', 'r', 'n', 'b', 'q', 'k'};

    row = col = 7;
    while (std::getline(ss, rank, '/')) {
        col = 7;
        for (char piece : rank) {
            if (isdigit(piece)) {
                col -= (piece - '0');
            } else {
                color = (islower(piece)) ? Black : White;
                piece = tolower(piece);

                for (int i = 0; i < 6; i++) {
                    if (piece == P[i]) {
                        BitManipulation::setBit(&board.positions[color][i], row*8 + col);
                        board.allPositions[color] |= board.positions[color][i];
                        board.board[row*8 + col] = (color == Black) ? piece : toupper(piece);
                        break;
                    }
                }
                col--;
            }
        }
        row--;       
    }
}

void Game::setCastleRight(std::string token) {
    board.castlingRights = 0;

    if (token.compare("-") == 0) 
        return;

    const char castle[4] = {'K', 'Q', 'k', 'q'};
    for (char c : token) {
        for (int j = 0; j < 4; j++) {
            if (c == castle[j]) {
                board.castlingRights |= (1 << j);
                break;
            }
        }
    }
}

void Game::setEnPassen(std::string token) {
    if (token.compare("-") == 0)
        return;

    int row, col;
    row = 7 - (token[1] - '1');
    col = 'h' - token[0]; // H1 is the LSB

    board.enPassen = row*8 + col;
}

void Game::calculateMoves(int side) {
    u64 map;
    int square;

    MoveGen::setPin(board, calBoard);
    calBoard.allPossibleAttacks[side] = 0ULL;
    calBoard.procPositions[side] = 0ULL;
    calBoard.checkSources[side] = 0ULL;
    calBoard.allLegalMoves[side] = 0ULL;
    for (int piece = 0; piece < 6; piece++) {
        if ((map = board.positions[side][piece]) == 0) 
            continue;
        while (map) {
            square = BitManipulation::getLSSB(map);
            BitManipulation::popBit(&map, square);
            MoveGen::genMoves(board, calBoard, piece, square);
        }
    }
}

void Game::reset() {
    memset(&board, 0, sizeof(board));
    memset(&calBoard, 0, sizeof(calBoard));
    if (!moveStacks.empty())
        moveStacks.clear();
}