import tkinter
import threading

class MainWindow(tkinter.Frame):
    def __init__(self, width, height):
        super().__init__()

        self.width = width
        self.height = height

        self.canvas = tkinter.Canvas()
        self.canvas.pack(fill=tkinter.BOTH, expand=True)

    def draw_rectangle(self, x, y, size=10):
        self.canvas.delete(tkinter.ALL)

        self.canvas.create_rectangle(0, 0, self.width, self.height, fill='black')
        self.canvas.create_rectangle(x, y, x + size, y + size, fill='red')


class GraphicsThread(threading.Thread):
    def run(self):
        self.window_width = 800
        self.window_height = 600

        self.root = tkinter.Tk()
        self.root.geometry(f'{self.window_width}x{self.window_height}')

        self.main_window = MainWindow(self.window_width, self.window_height)

        self.root.mainloop()
