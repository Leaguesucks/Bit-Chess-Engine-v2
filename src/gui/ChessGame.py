import subprocess
import tkinter as tk
from PIL import Image, ImageTk

root, canvas = None, None

begin = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" # Begin position
cPieces = ["P", "R", "N", "B", "Q", "K", "p", "r", "n", "b", "q", "k"]
pieceImg = {}
absPath = str()

def init():
    '''Initialize the GUI'''
    global root, canvas

    root = tk.Tk()
    root.title("Chess GUI")
    root.geometry("800x800+50+50")
    root.resizable(False, False)

    canvas = tk.Canvas(root, width=800, height=800)
    canvas.pack()

def drawChessBoard():
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

            canvas.create_rectangle(x1, y1, x2, y2, fill=color, outline="")

def loadPiecesImg():
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

def setFEN(fen: str):
    '''Set up the chess board from a FEN'''
    canvas.delete("pieces")

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
                    canvas.create_image(
                        x, y,
                        anchor="nw",
                        image=pieceImg[p],
                        tags="pieces"
                    )

                col += 1
        row += 1

def drawLabels():
    '''Draw rank (1–8) and file (a–h) labels'''

    files = ['a','b','c','d','e','f','g','h']
    ranks = ['8','7','6','5','4','3','2','1']

    for col in range(8):
        x = col * 100 + 5
        y = 800 - 5
        canvas.create_text(
            x, y,
            text=files[col],
            anchor="sw",
            fill="black",
            font=("Arial", 10)
        )

    for row in range(8):
        x = 5
        y = row * 100 + 5
        canvas.create_text(
            x, y,
            text=ranks[row],
            anchor="nw",
            fill="black",
            font=("Arial", 10)
        )

if __name__ == "__main__":
    init()
    drawChessBoard()
    drawLabels()
    loadPiecesImg()
    setFEN(begin)

    root.mainloop()