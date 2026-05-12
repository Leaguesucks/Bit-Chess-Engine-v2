#include "../headers/UCI.h"

UCI::UCI() {
    *game = Game(BEGIN);
}

UCI::UCI(Game* game) {
    this->game = game;
}

void UCI::send(const char* msg) {
    printf("%s\n", msg);
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
    while (true) {
        command = recv();
        if (command.compare("quit") == 0) {
            printf("Quitting!\n");
            return;
        }
        strncpy(cstr, command.c_str(), BUFFSIZE);
        processMsg(cstr);
        memset(cstr, 0, BUFFSIZE);
    }
}

void UCI::processMsg(char* msg) {
    char* token;

    if (strcmp(msg, "uci"))
        send("uci ok");
    token = strtok(msg, " ");

    if (strcmp(token, "position") == 0) {
        token = strtok(msg, " ");
    }
}