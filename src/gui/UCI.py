import subprocess

class UCI:
    '''Handle communication with the engine using UCI protocol'''
    
    def __init__(self):
        self.engine = subprocess(["../game/main.exe"],
                                 stdin=subprocess.PIPE,
                                 stdout=subprocess.PIPE,
                                 text=True,
                                 bufsize=1)