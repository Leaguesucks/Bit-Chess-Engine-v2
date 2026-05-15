from gui.ChessGame import ChessGame
from uci.UCI import UCI
import signal

def handleSend(signum, frame) -> None:
    '''Handle the event that a command is entered'''
    game.printConsole(game.command)
    uci.send(game.command)
    response = uci.recv()
    game.printConsole(response)
    processResponse(response)

def processResponse(response: str) -> None:
    '''Process the response received from the engine'''
    tokens = response.split("__")

    if tokens[0] == "position":
        game.setFEN(tokens[1])

if __name__ == "__main__":
    global uci, game

    signal.signal(signal.SIGUSR1, handleSend)


    uci = UCI()
    game = ChessGame(uci)
    game.execute()