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
    calculateMoves(board.side2play); // First calculate all POSSIBLE moves for the current side
    calculateMoves(board.side2play ^ 1); // Then, calculate moves for the previous side
    calculateMoves(board.side2play); // Then re-calculate for the current side against
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
                if (empty > 0) 
                    fen.append(std::to_string(empty));
                fen.append(1, board.board[row*8 + col]);
                empty = 0;                    
            }

            if (col == 0 && empty > 0) 
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

    (board.enPassen < 0) ? fen.append("-") : fen.append(SQUARE_STR[board.enPassen]);
    fen.append(" ");

    fen.append(std::to_string(100 - board.fifty));
    fen.append(" ");

    fen.append(std::to_string(board.fly));

    return fen;
}

GameData::CalBoard* Game::getCalBoard() {
    return &calBoard;
}

GameData::BitBoard* Game::getBoard() {
    return &board;
}

int Game::makeMove(GameData::MoveInfo moveInfo) {
    u8 piece, pieceNum;

    piece = board.board[moveInfo.from];
    if (piece == EMPTY)
        return INVALID_MOVE;

    for (pieceNum = 0; pieceNum < 6; pieceNum++)
        if (tolower(piece) == PIECE_CHAR[pieceNum]) 
            break;

    checkValidMove(moveInfo);
    if (moveInfo.status == INVALID_MOVE)
        return INVALID_MOVE;

    switch (pieceNum)
    {
        case PAWN:
            movePawn(moveInfo);
            break;
        case KING:
            moveKing(moveInfo);
            break;
        case ROOK:
            moveRook(moveInfo);
            break;
        default:      
            break;
    }

    postSuccessfulMove(moveInfo);
    return VALID_MOVE;
}

void Game::undo() {

}

void Game::setPosition(std::string token) {
    char piece;
    int row, col, color;
    std::string rank;
    std::stringstream ss(token);

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
                    if (piece == PIECE_CHAR[i]) {
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
    board.enPassen = -1;
    if (token.compare("-") == 0)
        return;

    int row, col;
    row = token[1] - '1';
    col = 'h' - token[0]; // H1 is the LSB

    board.enPassen = row*8 + col;
}

void Game::setEnPassen(int side, int des) {
    int squareAdd = (side == White) ? -8 : 8;
    board.enPassen = des + squareAdd;
}

int Game::getEnPassenVictim(int des, int side) {
    int squareAdd;

    squareAdd = (side == White) ? 8 : -8;
    return des + squareAdd;
}

void Game::calculateMoves(int side) {
    u64 map;
    int square;

    MoveGen::setPin(board, calBoard, side);
    calBoard.allPossibleAttacks[side] = 0ULL;
    calBoard.procPositions[side] = 0ULL;
    calBoard.checkSources[side] = 0ULL;
    calBoard.allLegalMoves[side] = 0ULL;
    for (int piece = 0; piece < 6; piece++) {
        if ((map = board.positions[side][piece]) == 0ULL) 
            continue;
        while (map) {
            square = BitManipulation::getLSSB(map);
            BitManipulation::popBit(&map, square);
            MoveGen::genMoves(board, calBoard, piece, square, side);
        }
    }
}

void Game::reset() {
    memset(&board, 0, sizeof(board));
    memset(&calBoard, 0, sizeof(calBoard));
    if (!moveStacks.empty())
        moveStacks.clear();
}

void Game::movePawn(GameData::MoveInfo& pMove) {
    int lastRank, fromRank, toRank;

    if (pMove.type == PROMOTION)
        return; // The GUI has handle the rest

    lastRank = (pMove.side == White) ? 7 : 0;
    fromRank = pMove.from / 8;
    toRank = pMove.to / 8;

    if (abs(toRank - fromRank) == 2) {
        pMove.type = PAWN_DOUBLE_MOVE;
        return;
    }

    if (toRank == board.enPassen) {
        pMove.type = ENPASSEN;
        return;
    }

    pMove.type = PAWN_MOVE;
}

void Game::moveKing(GameData::MoveInfo& pMove) {
    int side = pMove.side;

    if (pMove.to == LookupTable::KING_CASTLE_END_SQUARE[side][KING_SIDE]) {
        pMove.type = CASTLE;
        pMove.castleSide = KING_SIDE;
    } else if (pMove.to == LookupTable::KING_CASTLE_END_SQUARE[side][QUEEN_SIDE]) {
        pMove.type = CASTLE;
        pMove.castleSide = QUEEN_SIDE;
    } else {
        // Normal move -> Clear the castling flag
        board.castlingRights &= ~(1 << (2*side + KING_SIDE));
        board.castlingRights &= ~(1 << (2*side + QUEEN_SIDE));
    }
}

void Game::moveRook(GameData::MoveInfo& pMove) {
    if (BitManipulation::getBit(board.castlingRights, pMove.side * 2 + KING_SIDE) &&
        pMove.from == LookupTable::ROOK_CASTLE_START_SQUARE[pMove.side][KING_SIDE])
        board.castlingRights &= ~(1 << (pMove.side * 2 + KING_SIDE));
    
    if (BitManipulation::getBit(board.castlingRights, pMove.side * 2 + QUEEN_SIDE) &&
        pMove.from == LookupTable::ROOK_CASTLE_START_SQUARE[pMove.side][QUEEN_SIDE])
        board.castlingRights &= ~(1 << (pMove.side * 2 + QUEEN_SIDE));
}

void Game::encodeMove(const GameData::MoveInfo moveInfo) {
    u64 encode = 0ULL;

    encode |= moveInfo.side;
    encode |= moveInfo.piece << 1;
    encode |= moveInfo.from << 4;
    encode |= moveInfo.to << 10;
    encode |= moveInfo.type << 16;
    encode |= moveInfo.capture << 19;
    encode |= moveInfo.prevCastle << 22;
    encode |= moveInfo.promote << 26;
    
    moveStacks.push_back(encode);
}

void Game::postSuccessfulMove(const GameData::MoveInfo moveInfo) {
    int enemySide = moveInfo.side ^ 1;
    int side = moveInfo.side;
    char piece, promote;

    piece = (side == White) ? toupper(PIECE_CHAR[moveInfo.piece]) : PIECE_CHAR[moveInfo.piece];
    switch (moveInfo.type)
    {
        case CAPTURE:
            BitManipulation::popBit(&board.positions[enemySide][moveInfo.capture], moveInfo.to);
            board.fifty = FIFTY_INITIAL;
            break;
        case ENPASSEN:
            BitManipulation::popBit(&board.positions[enemySide][PAWN], getEnPassenVictim(moveInfo.to, enemySide));
            board.enPassen = -1;
            board.fifty = FIFTY_INITIAL;
            break;
        case PROMOTION:
            BitManipulation::popBit(&board.positions[side][PAWN], moveInfo.from);
            BitManipulation::setBit(&board.positions[side][moveInfo.promote], moveInfo.to);
            board.fifty = FIFTY_INITIAL;
            promote = (side == White) ? toupper(PIECE_CHAR[moveInfo.promote]) : PIECE_CHAR[moveInfo.promote];
            break;
        case CASTLE:
            BitManipulation::popBit(&board.positions[side][ROOK], LookupTable::ROOK_CASTLE_START_SQUARE[side][moveInfo.castleSide]);
            BitManipulation::setBit(&board.positions[side][ROOK], LookupTable::ROOK_CASTLE_END_SQUARE[side][moveInfo.castleSide]);
            board.castlingRights &= ~(1 << (side*2 + moveInfo.castleSide));
            board.fifty--;
            break;
        case PAWN_MOVE:
            board.fifty = FIFTY_INITIAL;
            break;
        case PAWN_DOUBLE_MOVE:
            board.fifty = FIFTY_INITIAL;
            setEnPassen(side, moveInfo.to);
            break;
        default:
            board.fifty--;
            break;
    }

    if (moveInfo.type != PROMOTION) {
        BitManipulation::popBit(&board.positions[side][moveInfo.piece], moveInfo.from);
        BitManipulation::setBit(&board.positions[side][moveInfo.piece], moveInfo.to);
        board.board[moveInfo.from] = EMPTY;
        board.board[moveInfo.to] = piece;
    } else {
        board.board[moveInfo.from] = EMPTY;
        board.board[moveInfo.to] = promote;
    }

    memset(board.allPositions, 0, sizeof(board.allPositions));
    for (int i = 0; i < 6; i++) {

        board.allPositions[White] |= board.positions[White][i];
        board.allPositions[Black] |= board.positions[Black][i];
    }

    if (moveInfo.side == Black)
        board.fly++;
    calculateMoves(moveInfo.side);
    board.side2play = enemySide;
    calculateMoves(enemySide);
    encodeMove(moveInfo);
}

void Game::checkValidMove(GameData::MoveInfo& pMove) {
    int enemySide = pMove.side ^ 1;
    int capturePiece;
    u64 toMask;

    toMask = 0ULL;
    BitManipulation::setBit(&toMask, pMove.to);
    if (calBoard.captures[pMove.from] & toMask) {
        pMove.type = CAPTURE;

        for (capturePiece = 0; capturePiece < 6; capturePiece++)
            if (board.positions[enemySide][capturePiece] & toMask) {
                pMove.capture = capturePiece;
                pMove.status = VALID_MOVE;
                return;
            }
    }
    else if (calBoard.moves[pMove.from] & toMask) {
        pMove.type = NORMAL;
        pMove.status = VALID_MOVE;
    }
    else
        pMove.status = INVALID_MOVE;
}