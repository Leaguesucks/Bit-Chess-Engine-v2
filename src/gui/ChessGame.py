import subprocess
import tkinter as tk
from PIL import Image, ImageTk

BEGIN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" # Begin position
CPIECES = ["P", "R", "N", "B", "Q", "K", "p", "r", "n", "b", "q", "k"]

class ChessGame:
    '''Launch the chess game GUI'''
    def __init__(self, uci):
        self.pieceImg = {}

        self.root = tk.Tk()
        self.canvas = tk.Canvas(self.root, width=800, height=800)
        self.root.title("Chess GUI")
        self.root.geometry("800x850+50+50")
        self.root.resizable(False, False)
        self.canvas.pack()

        self.pauseButn = tk.Button(
            self.root,
            text="Pause",
            command=self.openPauseMenu,
            font=("Arial", 12)
        )

        self.pauseButn.pack(pady=10)

    def drawChessBoard(self) -> None:
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

    def loadPiecesImg(self) -> None:
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
            img = Image.open(path + piece + ".png")
            img = img.resize((100, 100))
            self.pieceImg[CPIECES[count]] = ImageTk.PhotoImage(img)
            count += 1

    def drawLabels(self) -> None:
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

    def setFEN(self, fen: str) -> None:
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

                    if p in CPIECES:
                        self.canvas.create_image(
                            x, y,
                            anchor="nw",
                            image=self.pieceImg[p],
                            tags="pieces"
                        )

                    col += 1
            row += 1

    def openPauseMenu(self) -> None:
        '''Open a pause menu'''
        popUp = tk.Toplevel(self.root)

        popUp.title("Pause Menu")
        popUp.geometry("300x400")
        popUp.resizable(False, False)

        resumeBtn = tk.Button(
            popUp,
            text="Resume",
            width=20,
            height=2,
            command=popUp.destroy
        )
        resumeBtn.pack(pady=5)

        commandBtn = tk.Button(
            popUp,
            text="Command",
            width=20,
            height=2,
            command=self.openCommandConsole
        )
        commandBtn.pack(pady=5)

    def openCommandConsole(self) -> None:
        '''Open the command console'''
        cs = tk.Toplevel(self.root)

        cs.title("Command console")
        cs.geometry("500x400")
        cs.resizable(False, False)

        outputBox = tk.Text(
            cs,
            height=15,
            width=60
        )
        outputBox.pack(pady=10)

        entry = tk.Entry(
            cs,
            width=50,
            font=("Arial", 12)
        )
        entry.pack(pady=10)

        sendBtn = tk.Button(
            cs,
            text="Send",
            command=lambda: self.getConsoleCommand(entry, outputBox)
        )
        sendBtn.pack(pady=10)

        entry.bind(
            "<Return>",
            lambda event: self.getConsoleCommand(entry, outputBox)
        )

    def getConsoleCommand(self, entry: tk.Entry, output: tk.Text):
        '''Get the command input'''
        cmd = entry.get().strip()

        if cmd:
            output.insert(tk.END, f"> {cmd}\n")

        entry.delete(0, tk.END)

    def execute(self, fen: str = BEGIN):
        self.drawChessBoard()
        self.drawLabels()
        self.loadPiecesImg()
        self.setFEN(fen)

        self.root.mainloop()
