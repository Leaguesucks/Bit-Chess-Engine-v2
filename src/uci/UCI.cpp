#include "../headers/UCI.h"

UCI::UCI(Game* game) {
    this->game = game;
}

void UCI::send(std::string msg) {
    std::cout << msg << std::endl;
}

std::string UCI::recv() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void UCI::connect() {
    std::string command;
    
    // @todo Implement quiting the engine AFTER finishing the engine
    while (true) {
        command = recv();
        processMsg(command);
    }
}

void UCI::processMsg(std::string msg) {
    std::stringstream ss(msg);
    std::string token, response;
    int square;

    ss >> token;
    if (token.compare("uci") == 0) {
        response = "uciok";
    } else if (token.compare("position") == 0) {
        response = processPosition(msg);
    } else if (token.compare("showMove") == 0) {
        ss >> token;
        square = std::stoi(token);
        response.append("showMove__moves__");
        response.append(BitManipulation::encodeMaskBits(game->getCalBoard()->moves[square]));
        response.append("__captures__");
        response.append(BitManipulation::encodeMaskBits(game->getCalBoard()->captures[square]));
    } else if (token.compare("moves") == 0) {
        response = processMoves(msg); 
    } else
        response = ERROR_MSG;

    send(response);
}

std::string UCI::processPosition(std::string msg) {
    std::stringstream ss(msg);
    std::string token;
    std::string response("position__"); // For simplicity fields in a response are seperated by 2 underscores

    ss >> token; // "position"
    ss >> token; // startpos or fen

    if (token.compare("startpos") == 0) {
        game->setFEN(BEGIN);
    } else if (token.compare("fen") == 0) {
        std::string fen("");

        while ((ss >> token) && token.compare("moves") != 0) {
            fen.append(token); // Rebuild the fen
            fen.append(" ");
        }
        game->setFEN(fen);
    } else
        response = ERROR_MSG;

    while (ss >> token) {
        // @todo Implement later     
    } 

    response.append(game->toFEN());
    return response;
}

std::string UCI::processMoves(std::string move) {
    std::stringstream ss(move);
    std::string token,subres, response("position__");
    GameData::MoveInfo moveInfo;
    int moveStatus;

   ss >> token; // "moves"
   ss >> token; // "GUI" or the first move
   if (token.compare("GUI") == 0) {
        subres = processSimpleMoves(move);
        if (subres.compare(ERROR_MSG) == 0)
            return ERROR_MSG;
        response.append(subres);
        return response;
   } 

    return response;
}

std::string UCI::processSimpleMoves(std::string move) {
    std::stringstream ss(move);
    std::string token, response;
    GameData::MoveInfo moveInfo;

    ss >> token;
    ss >> token;

    while (ss >> token) {
        moveInfo = buildMove(token);
        if (game->makeMove(moveInfo) == INVALID_MOVE) {
            return ERROR_MSG;
        }
    }
    
    return game->toFEN();
}

GameData::MoveInfo UCI::buildMove(std::string move) {
    GameData::MoveInfo moveInfo;
    int srcRow, srcCol, desRow, desCol, piece;

    moveInfo.side = game->getBoard()->side2play;
    if (isupper(move[0])) { // Normal piece move
        srcCol = 'h' - move[1];
        srcRow = move[2] - '1';
        desCol = 'h' - move[3];
        desRow = move[4] - '1';

        for (piece = 0; piece < 6; piece++)
            if (tolower(move[0]) == PIECE_CHAR[piece])
                break;
    } else { // Pawn move
        srcCol = 'h' - move[0];
        srcRow = move[1] - '1';
        desCol = 'h' - move[2];
        desRow = move[3] - '1';

        if (move.length() > 4) // Promotion
            for (piece = 0; piece < 6; piece++)
                if (tolower(move[5]) == PIECE_CHAR[piece])
                    break;
        
        moveInfo.promote = piece;
        moveInfo.type = PROMOTION;
        piece = PAWN;
    }

    moveInfo.from = srcRow*8 + srcCol;
    moveInfo.to = desRow*8 + desCol;
    moveInfo.piece = piece;

    return moveInfo;
}