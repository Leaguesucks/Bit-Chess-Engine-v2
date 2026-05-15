#include "../headers/UCI.h"

UCI::UCI(Game* game) {
    this->game = game;
}

void UCI::send(const char* msg) {
    std::cout << msg << std::endl;
}

std::string UCI::recv() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void UCI::connect() {
    std::string command = "";
    char cstr[BUFFSIZE];

    memset(cstr, 0, BUFFSIZE);
    
    // @todo Implement quiting the engine AFTER finishing the engine
    while (true) {
        command = recv();
        
        strncpy(cstr, command.c_str(), BUFFSIZE - 1);
        processMsg(cstr);
    }
}

void UCI::processMsg(const char* msg) {
    char *token, cmsg[BUFFSIZE];

    memset(cmsg, 0, BUFFSIZE);
    strcpy(cmsg, msg);

    if (strcmp(cmsg, "uci") == 0) {
        send("uciok");
        return;
    }

    token = strtok(cmsg, " ");
    if (strcmp(token, "position") == 0) {
        processPosition(msg);
        return;
    }

    send(msg);
}

void UCI::processPosition(const char* msg) {
    char* token, cmsg[BUFFSIZE];
    std::string response("position__"); // For simplicity fields in a response are seperated by 2 underscores

    strcpy(cmsg, msg);
    token = strtok(cmsg, " "); // "position"
    token = strtok(NULL, " "); // startpos or fen

    if (strcmp(token, "startpos") == 0) {
        game->setFEN(BEGIN);

        token = strtok(NULL, " ");
    } else if (strcmp(token, "fen") == 0) {
        std::string fen("");

        while ((token = strtok(NULL, " ")) && strcmp(token, "moves") != 0)
            fen.append(token); // Rebuild the fen
        game->setFEN(fen.c_str());
    }

    if (token != NULL && strcmp(token, " moves") == 0) {
        // @todo Implement later     
    } 

    response.append(game->toFEN());
    send(response.c_str());
}