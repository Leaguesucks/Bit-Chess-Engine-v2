#include "../headers/Game.h"

Game::Game() {
    memset(&board, 0, sizeof(board));
    memset(&calBoard, 0, sizeof(calBoard));
}

Game::Game(char* fen) {
    Game();
    setFEN(fen);
}

void Game::setFEN(char* fen) {
    char* token;

    u8 count = 0;
    while ((token = strtok(fen, " "))) {
        switch (count) {
        case 0: // Placement positon
            setPosition(token);
            break;
        case 1: // Active color
            board.side2play = (token[0] == 'w') ? W : B;
            break;
        case 2: // Castle right
            setCastleRight(token);
            break;
        case 3: // EnPassen square
            setEnPassen(token);
            break;
        case 4: // Halfmove clock
            calBoard.fifty = 100 - strtol(token, NULL, 10);
            break;
        case 5: // Fullmove number
            board.fly = strtol(token, NULL, 10);
            break;
        default:
            perror("Invalid number of field!");
            return;
        }
        
        count++;
    }
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

u64 Game::getMovesOnSquare(int square) {
    return calBoard.moves[square];
}

u64 Game::getCapturesOnSquare(int square) {
    return calBoard.captures[square];
}

void Game::setPosition(char* token) {
    char piece, *rank;
    int row, col, color;
    const char P[6] = {'p', 'r', 'n', 'b', 'q', 'k'};

    row = col = 0;
    while ((rank = strtok(token, "/"))) {
        col = 0;
        for (int p = 0; (piece = rank[p]) != '\0'; p++) {
            if (isdigit(piece)) {
                col += (piece - '0');
                continue;
            }

            color = (islower(piece)) ? B : W;
            piece = tolower(piece);
            
            for (int i = 0; i < 6; i++) {
                if (piece == P[i]) {
                    BitManipulation::setBit(&board.positions[color][i], row*8 + col);
                    board.board[row*8 + col] = (color == B) ? piece : toupper(piece);
                    break;
                }
            }

            col++;
        }
        rank++;
    }
}

void Game::setCastleRight(char* token) {
    board.castlingRights = 0;

    if (strcmp("-", token) == 0) 
        return;

    char c;
    const char castle[4] = {'K', 'Q', 'k', 'q'};
    for (int i = 0; (c = token[i]) != '\0'; i++) {
        for (int j = 0; j < 4; j++) {
            if (c == castle[j]) {
                board.castlingRights ^= (1 << j);
                break;
            }
        }
    }
}

void Game::setEnPassen(char* token) {
    calBoard.enPassen = 0;

    if (strcmp("-", token) == 0)
        return;

    int row, col;
    row = token[1] - '1';
    col = 'h' - token[0]; // H1 is the LSB

    BitManipulation::setBit(&calBoard.enPassen, row*8 + col);
}