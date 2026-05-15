from gui.ChessGame import ChessGame
from uci.UCI import UCI

if __name__ == "__main__":
    uci = UCI()
    game = ChessGame(uci)
    game.execute()