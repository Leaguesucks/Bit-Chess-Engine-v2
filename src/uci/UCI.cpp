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
    std::string token;

    while (ss >> token) {
        if (token.compare("uci") == 0) {
            send("uciok");
            return;
        } else if (token.compare("position") == 0) {
            processPosition(msg);
            return;
        }
    }

    send(msg);
}

void UCI::processPosition(std::string msg) {
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

    if (ss >> token) {
        // @todo Implement later     
    } 

    response.append(game->toFEN());
    send(response.c_str());
}