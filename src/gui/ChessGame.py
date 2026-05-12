import subprocess
import tkinter as tk
from src.uci.UCI import UCI
from PIL import Image, ImageTk

begin = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" # Begin position
cPieces = ["P", "R", "N", "B", "Q", "K", "p", "r", "n", "b", "q", "k"]
pieceImg = {}
absPath = str()

class ChessGame:
    '''Launch the chess game GUI'''
    def __init__(self):
        self.root = tk.Tk()
        self.canvas = tk.Canvas(self.root, width=800, height=800)
        self.root.title("Chess GUI")
        self.root.geometry("900x900+50+50")
        self.root.resizable(False, False)
        self.canvas.pack()

        self.pauseButn = tk.Button(
            self.root,
            text="Pause",
            command=self.openConsole,
            font=("Arial", 12)
        )

        self.pauseButn.pack(pady=10)

    def drawChessBoard(self):
        LIGHT = "#F0D9B5"
        DARK = "#B58863"

        for row in range(8):
            for col in range(8):
                x1 = col * 100
                y1 = row * 100
                x2 = x1 + 100
                y2 = y1 + 100

                color = None
                if (row + col) % 2 == 0:
                    color = LIGHT
                else:
                    color = DARK

                self.canvas.create_rectangle(x1, y1, x2, y2, fill=color, outline="")

    def loadPiecesImg(self):
        '''Load the pieces image'''
        pieces = [
            "whitepawn",
            "whiterook",
            "whiteknight",
            "whitebishop",
            "whitequeen",
            "whiteking",
            "blackpawn",
            "blackrook",
            "blackknight",
            "blackbishop",
            "blackqueen",
            "blackking"
        ]

        path = "resources/images/pieces/"

        count = 0
        for piece in pieces:
            img = Image.open(absPath + path + piece + ".png")
            img = img.resize((100, 100))
            pieceImg[cPieces[count]] = ImageTk.PhotoImage(img)
            count += 1

    def drawLabels(self):
        '''Draw rank (1–8) and file (a–h) labels'''

        files = ['a','b','c','d','e','f','g','h']
        ranks = ['8','7','6','5','4','3','2','1']

        for col in range(8):
            x = col * 100 + 5
            y = 800 - 5
            self.canvas.create_text(
                x, y,
                text=files[col],
                anchor="sw",
                fill="black",
                font=("Arial", 10)
            )

        for row in range(8):
            x = 5
            y = row * 100 + 5
            self.canvas.create_text(
                x, y,
                text=ranks[row],
                anchor="nw",
                fill="black",
                font=("Arial", 10)
            )

    def setFEN(self, fen: str):
        '''Set up the chess board from a FEN first field'''
        self.canvas.delete("pieces")

        field = fen.split()
        placement = field[0]
        ranks = placement.split("/")

        row, col = 0, 0
        for rank in ranks:
            col = 0
            for p in rank:
                if p.isdigit():
                    col += int(p)
                else:
                    y = row * 100
                    x = col * 100

                    if p in cPieces:
                        self.canvas.create_image(
                            x, y,
                            anchor="nw",
                            image=pieceImg[p],
                            tags="pieces"
                        )

                    col += 1
            row += 1

    def openConsole(self):
        '''Open pop up console to send command to the engine'''
        popUp = tk.Toplevel(self.root)

        popUp.title("Pause Menu")
        popUp.geometry("500x400")
        popUp.resizable(False, False)

        self.outputBox = tk.Text(
            popUp,
            height=15,
            width=60
        )

        self.outputBox.pack(pady=10)

        entry = tk.Entry(
            popUp,
            width=50,
            font=("Arial", 12)
        )

        entry.pack(pady=10)
        
        sendBtn = tk.Button(
            popUp,
            text="Enter",
            command=lambda: self.handleConsoleCommand(entry)
        )
        sendBtn.pack()

        entry.bind(
            "<Return>",
            lambda event: self.handleConsoleCommand(entry)
        )

    def handleConsoleCommand(self, entry: tk.Entry):
        '''Handle the command input'''
        cmd = entry.get().strip()

        if cmd:
            self.outputBox.insert(tk.END, f"> {cmd}\n")

        entry.delete(0, tk.END)

    def execute(self, fen: str):
        self.drawChessBoard()
        self.drawLabels()
        self.loadPiecesImg()
        self.setFEN(fen)

        self.root.mainloop()

if __name__ == "__main__":
    game = ChessGame()
    game.execute(begin)
