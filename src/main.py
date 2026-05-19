from gui.ChessGame import ChessGame
from uci.UCI import UCI
import signal

def handleConsoleSend(signum, frame) -> None:
    '''Handle the event that a command is entered'''
    game.printConsole(game.command)
    uci.send(game.command)
    response = uci.recv()
    game.printConsole(response)
    processResponse(response)

def handleGUIsend(signum, frame) -> None:
    '''Handle the event that a command is sent by the GUI process itself'''
    print(f">{game.command}") # debug
    uci.send(game.command)
    response = uci.recv()
    print(f">{response}") # debug
    processResponse(response)

def processResponse(response: str) -> None:
    '''Process the response received from the engine'''
    tokens = response.split("__")

    if tokens[0] == "position":
        game.render(tokens[1], "", "")
    elif tokens[0] == "showMove":
        game.render("", tokens[2], tokens[4])

if __name__ == "__main__":
    global uci, game

    signal.signal(signal.SIGUSR1, handleConsoleSend)
    signal.signal(signal.SIGUSR2, handleGUIsend)

    uci = UCI()
    game = ChessGame(uci)
    game.execute()