import subprocess

class UCI:
    '''Handle communication with the engine using UCI protocol'''
    
    def __init__(self):
        self.game = subprocess(["../game/main.exe"],
                                 stdin=subprocess.PIPE,
                                 stdout=subprocess.PIPE,
                                 text=True,
                                 bufsize=1)
    def send(self, msg: str):
        '''Send commands to the engine'''
        if self.game.stdin:
            self.game.stdin.wrtie(msg + "\n")
            self.game.stdin.flush()

    def recv(self) -> str:
        '''Read the response from the engine'''
        if self.game.stdout:
            return self.game.stdout.readline().strip()
        return "No Message"
