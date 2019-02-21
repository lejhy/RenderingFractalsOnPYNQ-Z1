import Mandelbrot


class Mandelbrot_CPP:
    def __init__(self):
        self.img_width = 1920
        self.img_height = 1080
        self.plot_width = 3.5
        self.plot_height = 2.0
        self.plot_x_min = -2.5
        self.plot_y_max = 1.0
        self.max_iteration = 1000

    def calculate(self):
        return Mandelbrot.calculate(
            self.img_width,
            self.img_height,
            self.plot_width,
            self.plot_height,
            self.plot_x_min,
            self.plot_y_max,
            self.max_iteration
        )


