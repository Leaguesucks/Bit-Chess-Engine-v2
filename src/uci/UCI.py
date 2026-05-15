import subprocess
import os

class UCI:
    '''Handle communication with the engine using UCI protocol'''
    
    def __init__(self):
        self.game = subprocess.Popen(["bin/main.exe"],
                                    stdin=subprocess.PIPE,
                                    stdout=subprocess.PIPE,
                                    stderr=subprocess.PIPE,
                                    text=True,
                                    bufsize=1)
        # subprocess.run(["./bin/main.exe"])

    def send(self, msg: str) -> None:
        '''Send commands to the engine'''
        self.game.stdin.write(msg + "\n")

    def recv(self) -> str:
        '''Read the response from the engine'''
        if self.game.stdout:
            return self.game.stdout.readline().strip()
        return "No Message"