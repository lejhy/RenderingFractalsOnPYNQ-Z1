import Mandelbrot


class Mandelbrot_CPP:
    def __init__(self):
        self._img_width = 1920
        self._img_height = 1080
        self._plot_width = 3.5
        self._plot_height = 2.0
        self._plot_x_min = -2.5
        self._plot_y_max = 1.0
        self._max_iteration = 1000
        self._colour_span = 8
        self._colour_0 = 0x00ffb444
        self._colour_1 = 0x007e00af
        self._colour_2 = 0x00200042
        self._colour_3 = 0x00FFFFFF
        self._shell = False

    @property
    def shell(self):
        return self._shell

    @shell.setter
    def shell(self, value):
        self._shell = value
        self.plot_width = self._plot_width
        self.plot_height = self._plot_height
        self.plot_x_min = self._plot_x_min
        self.plot_y_max = self._plot_y_max
        self.max_iteration = self._max_iteration

    @property
    def img_width(self):
        return self._img_width

    @img_width.setter
    def img_width(self, value):
        self._img_width = value

    @property
    def img_height(self):
        return self._img_height

    @img_height.setter
    def img_height(self, value):
        self._img_height = value

    @property
    def plot_width(self):
        return self._plot_width

    @plot_width.setter
    def plot_width(self, value):
        self._plot_width = value
        if self._shell:
            self._shell.send("mandelbrot.set_plot_width(%.15f)\n" % self._plot_width)

    @property
    def plot_height(self):
        return self._plot_height

    @plot_height.setter
    def plot_height(self, value):
        self._plot_height = value
        if self._shell:
            self._shell.send("mandelbrot.set_plot_height(%.15f)\n" % self._plot_height)

    @property
    def plot_x_min(self):
        return self._plot_x_min

    @plot_x_min.setter
    def plot_x_min(self, value):
        self._plot_x_min = value
        if self._shell:
            self._shell.send("mandelbrot.set_plot_x_min(%.15f)\n" % self._plot_x_min)

    @property
    def plot_y_max(self):
        return self._plot_y_max

    @plot_y_max.setter
    def plot_y_max(self, value):
        self._plot_y_max = value
        if self._shell:
            self._shell.send("mandelbrot.set_plot_y_max(%.15f)\n" % self._plot_y_max)

    @property
    def max_iteration(self):
        return self._max_iteration

    @max_iteration.setter
    def max_iteration(self, value):
        self._max_iteration = value
        if self._shell:
            self._shell.send("mandelbrot.set_max_iteration(%i)\n" % self._max_iteration)

    @property
    def colour_span(self):
        return self._colour_span

    @colour_span.setter
    def colour_span(self, value):
        self._colour_span = value
        if self._shell:
            self._shell.send("mandelbrot.set_colour_span(%i)\n" % self._colour_span)

    @property
    def colour_0(self):
        return self._colour_0

    @colour_0.setter
    def colour_0(self, value):
        self._colour_0 = value
        if self._shell:
            self._shell.send("mandelbrot.set_colour_0(%i)\n" % self._colour_0)

    @property
    def colour_1(self):
        return self._colour_1

    @colour_1.setter
    def colour_1(self, value):
        self._colour_1 = value
        if self._shell:
            self._shell.send("mandelbrot.set_colour_1(%i)\n" % self._colour_1)

    @property
    def colour_2(self):
        return self._colour_2

    @colour_2.setter
    def colour_2(self, value):
        self._colour_2 = value
        if self._shell:
            self._shell.send("mandelbrot.set_colour_2(%i)\n" % self._colour_2)

    @property
    def colour_3(self):
        return self._colour_3

    @colour_3.setter
    def colour_3(self, value):
        self._colour_3 = value
        if self._shell:
            self._shell.send("mandelbrot.set_colour_3(%i)\n" % self._colour_3)

    def calculate(self):
        if self.shell:
            self._shell.send("renderer.render_to_hdmi()\n")
        return Mandelbrot.calculate(
            self._img_width,
            self._img_height,
            self._plot_width,
            self._plot_height,
            self._plot_x_min,
            self._plot_y_max,
            self._max_iteration,
            self._colour_span,
            self._colour_0,
            self._colour_1,
            self._colour_2,
            self._colour_3
        )


