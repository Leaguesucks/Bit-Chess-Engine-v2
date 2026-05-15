#include "../headers/Game.h"

Game::Game() {
    memset(&board, 0, sizeof(board));
    memset(&calBoard, 0, sizeof(calBoard));
}

Game::Game(const char* fen) : Game() {
    setFEN(fen);
}

void Game::setFEN(const char* fen) {
    char* token, fencp[100];
    u8 count;

    strncpy(fencp, fen, sizeof(fencp));
    memset(&board.board, EMPTY, 64);
    board.enPassen = -1;
    count = 0;
    token = strtok(fencp, " ");
    while (token) {
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
            board.fifty = 100 - strtol(token, NULL, 10);
            break;
        case 5: // Fullmove number
            board.fly = strtol(token, NULL, 10);
            break;
        default:
            perror("Invalid number of field!");
            return;
        }
        
        count++;
        token = strtok(NULL, " ");
    }
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
                if (col == 0) fen.append(std::to_string(empty));
            } else {
                if (empty > 0) fen.append(std::to_string(empty));
                fen.append(1, board.board[row*8 + col]);
                empty = 0;                    
            }
        }
        (row > 0) ? fen.append("/") : fen.append(" ");
    }

    (board.side2play == W) ? fen.append("w ") : fen.append("b ");

    for (int c = 0; c < 4; c++) { // Castle right
        if (board.castlingRights == 0) {
            fen.append("-");
            break;
        }   
        if ((board.castlingRights >> c) & 1)
            fen.append(std::to_string(castleSeq[c]));
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

    row = col = 7;
    rank = strtok(token, "/");
    while (rank) {
        col = 7;
        for (int p = 0; (piece = rank[p]) != '\0'; p++) {
            if (isdigit(piece)) {
                col -= (piece - '0');
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

            col--;
        }
        row--;
        rank = strtok(NULL, "/");        
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
    if (strcmp("-", token) == 0)
        return;

    int row, col;
    row = token[1] - '1';
    col = 'h' - token[0]; // H1 is the LSB

    board.enPassen = row*8 + col;
}