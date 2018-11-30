import tkinter
import threading
import time


class MainWindow(tkinter.Frame):
    clear_color = 'black'

    def __init__(self, width, height):
        super().__init__()

        self.width = width
        self.height = height

        self.canvas = tkinter.Canvas()
        self.canvas.pack(fill=tkinter.BOTH, expand=True)

        self.clear_canvas()

    def clear_canvas(self):
        self.canvas.delete(tkinter.ALL)
        self.canvas.create_rectangle(0, 0, self.width, self.height, fill=self.clear_color)

    def draw_square(self, x, y, size=10):
        first_x = x + self.width / 2
        first_y = y + self.height / 2
        second_x = x + self.width / 2 + size
        second_y = y + self.height / 2 + size

        self.canvas.create_rectangle(first_x, first_y, second_x, second_y, fill='red')

    def animate_square(self, x, y, size=10):
        self.clear_canvas()
        self.draw_square(x, y, size)


class Graphics:
    def __init__(self, window_width, window_height):
        self.thread = threading.Thread(target=self.thread_function)
        self.thread.daemon = True
        self.window_width = window_width
        self.window_height = window_height
        self.root = None
        self.window = None

    def start(self):
        self.thread.start()

    def stop(self):
        self.root.quit()
        self.root.destroy()
        self.thread.join()

    def thread_function(self):
        self.root = tkinter.Tk()
        self.root.geometry(f'{self.window_width}x{self.window_height}')
        self.window = MainWindow(self.window_width, self.window_height)
        self.root.mainloop()
