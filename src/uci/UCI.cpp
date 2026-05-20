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
        printBoard(game->getCalBoard()->moves[square], std::cerr); // DEBUG
        response.append("__captures__");
        response.append(BitManipulation::encodeMaskBits(game->getCalBoard()->captures[square]));
    }

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

        ss >> token;
    } else if (token.compare("fen") == 0) {
        std::string fen("");

        while ((ss >> token) && token.compare("moves") != 0)
            fen.append(token); // Rebuild the fen
        game->setFEN(fen);
    }

    while (ss >> token) {
        // @todo Implement later     
    } 

    response.append(game->toFEN());
    return response;
}