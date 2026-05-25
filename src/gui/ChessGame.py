import signal
import math
import tkinter as tk
from PIL import Image, ImageTk

BEGIN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" # Begin position
CPIECES = ["P", "R", "N", "B", "Q", "K", "p", "r", "n", "b", "q", "k"]
EMPTYSTR = "EMPTY"
MASKSTR = "MASK"

SQUARE_STR = [
    "h1", "g1", "f1", "e1", "d1", "c1", "b1", "a1",
    "h2", "g2", "f2", "e2", "d2", "c2", "b2", "a2",
    "h3", "g3", "f3", "e3", "d3", "c3", "b3", "a3",
    "h4", "g4", "f4", "e4", "d4", "c4", "b4", "a4",
    "h5", "g5", "f5", "e5", "d5", "c5", "b5", "a5",
    "h6", "g6", "f6", "e6", "d6", "c6", "b6", "a6",
    "h7", "g7", "f7", "e7", "d7", "c7", "b7", "a7",
    "h8", "g8", "f8", "e8", "d8", "c8", "b8", "a8",
]

FILES_STR = ["h", "g", "f", "e", "d", "c", "b", "a"]
RANK_STR = ["1", "2", "3", "4", "5", "6", "7", "8"]

DETAILS_FIELD = ["Side to play", "Castling right", "Enpassen square", "Halfmove clock",
                  "Fullmove number"]
SIDE_FIELD = 0
CASTLE_FIELD = 1
ENPASSEN_FIELD = 2
HALFMOVE_FIELD = 3
FULLMOVE_FIELD = 4

class ChessGame:
    '''Launch the chess game GUI'''
    def __init__(self, uci):
        self.pieceImg = {}
        self.pieceMap = {} # Keep track of the where the pieces are
        self.board : list[str] = [EMPTYSTR] * 64
        self.gameDetails = {}

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
        self.command : str = None
        self.output : tk.Text = None
        self.entry : tk.Entry = None

        self.current_fen = str()

        self.pauseMenu : tk.Toplevel = None
        self.commandWindow : tk.Toplevel = None
        self.detailWindow : tk.Toplevel = None
        self.promoteWindow : tk.Toplevel = None

        self.selectedSquare = None
        self.selectedPieceImgObj = None

        self.pendingPromotion : list[int, str] = None

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

    def loadImg(self) -> None:
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

        pathPieces = "resources/images/pieces/"

        count = 0
        for piece in pieces:
            img = Image.open(pathPieces + piece + ".png")
            img = img.resize((100, 100))
            self.pieceImg[CPIECES[count]] = ImageTk.PhotoImage(img)
            count += 1

    def drawLabels(self) -> None:
        '''Draw rank (1–8) and file (a–h) labels'''
        for col in range(8):
            x = col * 100 + 5
            y = 800 - 5
            self.canvas.create_text(
                x, y,
                text=FILES_STR[7-col],
                anchor="sw",
                fill="black",
                font=("Arial", 10)
            )

        for row in range(8):
            x = 5
            y = row * 100 + 5
            self.canvas.create_text(
                x, y,
                text=RANK_STR[7-row],
                anchor="nw",
                fill="black",
                font=("Arial", 10)
            )

    def setFEN(self, fen: str) -> None:
        '''Set up the chess board from a FEN first field'''
        if len(fen) == 0:
            return

        self.canvas.delete("pieces")
        self.current_fen = fen

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
                        square = (7 - row)*8 + (7 - col)

                        piece = self.canvas.create_image(
                            x, y,
                            anchor="nw",
                            image=self.pieceImg[p],
                            tags=("pieces", "draggable")
                        )

                        self.pieceMap[piece] = square

                        self.canvas.tag_bind(
                            piece,
                            "<Button-1>",
                            lambda event, piece=piece: self.onPieceClick(event, piece)
                        )

                        self.board[(7-row)*8 + (7-col)] = p

                    col += 1
            row += 1
        
        activeColor = field[1]
        self.gameDetails[DETAILS_FIELD[SIDE_FIELD]] =  "White" if activeColor == "w" else "Black"

        castlingRight = field[2]
        castleStr = ""
        if castlingRight != "-":
            castleStr += castlingRight
        self.gameDetails[DETAILS_FIELD[CASTLE_FIELD]] = castleStr
        
        self.gameDetails[DETAILS_FIELD[ENPASSEN_FIELD]] = "None" if field[3] == "-" else field[3]
        self.gameDetails[DETAILS_FIELD[HALFMOVE_FIELD]] = field[4]
        self.gameDetails[DETAILS_FIELD[FULLMOVE_FIELD]] = field[5]


    def onPieceClick(self, event: tk.Event, piece: int) -> None:
        '''The actions to take when a piece is clicked'''
        self.selectedPieceImgObj = piece
        self.selectedSquare = self.pieceMap[piece]

        self.command = f"showMove {self.selectedSquare}"
        signal.raise_signal(signal.SIGUSR2)

    def onDestinationClick(self, event : tk.Event) -> None:
        '''Handle destination square click'''
        def local_Cleanup() -> None:
            for row in range(0,8):
                for col in range(0,8):
                    if self.board[row*8 + col] == MASKSTR:
                        self.board[row*8 + col] = EMPTYSTR

            self.selectedPieceImgObj = None
            self.selectedSquare = None
        
            self.canvas.delete("grey_masks")
            self.canvas.delete("orange_mask")
            self.command = None

        col = event.x // 100
        row = event.y // 100

        target = (7 - row)*8 + (7 - col)
        if self.selectedSquare == target:
            return
        if self.board[target] != MASKSTR:
            local_Cleanup()
            return

        moveStr = self.encodeMove(self.selectedSquare, 
                                      target, 
                                      self.board[self.selectedSquare])
        if moveStr is None:
            local_Cleanup()
            return
        self.command = f"moves GUI {moveStr}"
        print(self.command) # debug
        signal.raise_signal(signal.SIGUSR2)
        local_Cleanup()

    def encodeMove(self, src: int, des: int, piece: str) -> str:
        '''Encode a move in Algebraic Notation'''
        move = str()

        if self.board[des] != MASKSTR:
            return None
        
        side = "Black" if self.board[src].islower() else "White"
        if side != self.gameDetails[DETAILS_FIELD[SIDE_FIELD]]:
            return None

        if piece.upper() == "P":
            lastRank = 0 if side == "Black" else 7
            if des // 8 == lastRank: # Promotion
                # Handle promotion
                self.pendingPromotion = [des, piece]
                self.openPromotionWindow()
                move = f"{SQUARE_STR[src]}{SQUARE_STR[des]}={self.pendingPromotion[1]}"
                self.pendingPromotion = None
            else:
                move = f"{SQUARE_STR[src]}{SQUARE_STR[des]}"
        else:
            # For simplicity, encoded as [piece][from][to] e.g., Qa3a8
            move = f"{piece.upper()}{SQUARE_STR[src]}{SQUARE_STR[des]}"        

        return move


    def maskSquares(self, color: str, encode: str) -> None:
        '''Decode the mask string and mask it on the GUI'''
        self.canvas.delete(f"{color}_masks")
        if len(color) == 0 or len(encode) == 0:
            return
        
        emptyCol = 0
        board = encode.split("/")

        rowNum = 0
        for rowStr in board:
            for p in rowStr:
                if p.isdigit():
                    emptyCol += int(p)
                else:
                    row = rowNum * 100 + 50
                    col = emptyCol * 100 + 50
                    radius = 15
    
                    self.board[(7-rowNum)*8 + (7-emptyCol)] = MASKSTR
                    self.canvas.create_oval(
                        col - radius,
                        row - radius,
                        col + radius,
                        row + radius,
                        fill=color,
                        outline="",
                        tags=(f"{color}_masks", "masks")
                    )
                    emptyCol += 1 # Re-use of variable for convenience
            emptyCol = 0
            rowNum += 1

    def openPauseMenu(self) -> None:
        '''Open a pause menu'''
        if self.pauseMenu is not None and self.pauseMenu.winfo_exists():
            return

        self.pauseMenu = tk.Toplevel(self.root)

        self.pauseMenu.title("Pause Menu")
        self.pauseMenu.geometry("300x400")
        self.pauseMenu.resizable(False, False)

        resumeBtn = tk.Button(
            self.pauseMenu,
            text="Resume",
            width=20,
            height=2,
            command=self.closePauseMenu
        )
        resumeBtn.pack(pady=5)

        commandBtn = tk.Button(
            self.pauseMenu,
            text="Command",
            width=20,
            height=2,
            command=self.openCommandConsole
        )
        commandBtn.pack(pady=5)

        detailsBtn = tk.Button(
            self.pauseMenu,
            text="Details",
            width=20,
            height=2,
            command=self.openDetailWindow
        )
        detailsBtn.pack(pady=5)

    def closePauseMenu(self) -> None:
        '''Close the pause menu'''
        if self.pauseMenu is not None:
            self.pauseMenu.destroy()
            self.pauseMenu = None

    def openCommandConsole(self) -> None:
        '''Open the command console'''

        if self.commandWindow is not None and self.commandWindow.winfo_exists():
            return
        if self.pauseMenu is not None: # Hide the pause menu
            self.pauseMenu.withdraw()

        self.commandWindow = tk.Toplevel(self.root)
        self.commandWindow.protocol("WM_DELETE_WINDOW", self.closeExtraConsole)

        self.commandWindow.title("Command console")
        self.commandWindow.geometry("500x400")
        self.commandWindow.resizable(False, False)

        outputBox = tk.Text(
            self.commandWindow,
            height=15,
            width=60
        )
        outputBox.pack(pady=10)
        self.output = outputBox

        entry = tk.Entry(
            self.commandWindow,
            width=50,
            font=("Arial", 12)
        )
        entry.pack(pady=10)
        self.entry = entry

        sendBtn = tk.Button(
            self.commandWindow,
            text="Send",
            command=lambda: self.getConsoleCommand()
        )
        sendBtn.pack(pady=10)

        entry.bind(
            "<Return>",
            lambda event: self.getConsoleCommand()
        )

    def closeExtraConsole(self) -> None:
        '''Close extra consoles except the Pause Menu'''

        if self.commandWindow is not None:
            self.commandWindow.destroy()
            self.commandWindow = None

        if self.detailWindow is not None:
            self.detailWindow.destroy()
            self.detailWindow = None

        if self.pauseMenu is not None and self.pauseMenu.winfo_exists():
            self.pauseMenu.deiconify() # Re-show the menu window

    def getConsoleCommand(self) -> None:
        '''Get the command input'''
        cmd = self.entry.get().strip()

        if cmd:
            self.command = cmd
            signal.raise_signal(signal.SIGUSR1) # Notice the process that a command has been issued

    def printConsole(self, cmd: str) -> None:
        '''Print a command on the command console'''
        self.output.insert(tk.END, f">{cmd}\n")
        self.entry.delete(0, tk.END)

    def openDetailWindow(self) -> None:
        '''Open a detail window that contains the game details'''
        if self.detailWindow is not None and self.detailWindow.winfo_exists():
            return
        if self.pauseMenu is not None:
            self.pauseMenu.withdraw()

        self.detailWindow = tk.Toplevel(self.root)
        self.detailWindow.protocol("WM_DELETE_WINDOW", self.closeExtraConsole)
        self.detailWindow.title("Game Details")
        self.detailWindow.geometry("500x500")
        self.detailWindow.resizable(False, False)

        HEIGHT, WIDTH = 1, 25
        for attribute in DETAILS_FIELD:
            attr = tk.Text(self.detailWindow, 
                           height=HEIGHT, 
                           width=WIDTH, 
                           )
            attr.pack(pady=5)
            attr.insert(tk.END, f"{attribute}: {self.gameDetails[attribute]}")


    def openPromotionWindow(self) -> None:
        if self.promoteWindow is not None and self.promoteWindow.winfo_exists():
            return

        self.promoteWindow = tk.Toplevel(self.root)
        self.promoteWindow.title("Promotion")
        self.promoteWindow.geometry("300x200")
        self.promoteWindow.resizable(False, False)

        for p in ["Q", "R", "B", "N"]:
            tk.Button(
                self.promoteWindow,
                text=p,
                width=10,
                command=lambda p=p: self.selectPromotion(p)
            )

        self.promoteWindow.protocol("WM_DELETE_WINDOW", lambda: None)

    def selectPromotion(self, promoPiece: str) -> None:
        if self.pendingPromotion is None:
            return
        
        self.pendingPromotion = [self.pendingPromotion[0], promoPiece]
    

    def render(self, fen: str, moves: str, captures: str) -> None:
        '''Render a new frame'''
        self.setFEN(fen)
        self.maskSquares("grey", moves)
        self.maskSquares("orange", captures)

        self.canvas.tag_raise("pieces")
        self.canvas.tag_raise("grey_masks")
        self.canvas.tag_raise("orange_masks")

    def execute(self, fen: str = BEGIN) -> None:
        self.loadImg()
        self.drawChessBoard()
        self.drawLabels()
        self.render(fen, "", "")
        self.canvas.bind("<Button-1>", self.onDestinationClick)

        self.root.mainloop()
