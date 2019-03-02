import Mandelbrot
import numpy as np


class Mandelbrot_CPP:
    def __init__(self):
        self._img_width = 1920
        self._img_height = 1080
        self._width_fraction = 3.5 / self._img_width
        self._height_fraction = 2.0 / self._img_height
        self._plot_x_min = -2.5
        self._plot_y_max = 1.0
        self._max_iteration = 1000
        self._colour_offset = 0
        self._colour_span = 8
        self._colours = np.array(
            [0x00ffb444, 0x007e00af, 0x00200042, 0x00FFFFFF, 0x00FFFFFF],
            np.uint32
        )
        self._shell = False

    @property
    def shell(self):
        return self._shell

    @shell.setter
    def shell(self, value):
        self._shell = value
        self.width_fraction = self._width_fraction
        self.height_fraction = self._height_fraction
        self.plot_x_min = self._plot_x_min
        self.plot_y_max = self._plot_y_max
        self.max_iteration = self._max_iteration
        self.width_fraction = self._width_fraction
        self.height_fraction = self._height_fraction
        self.colour_offset = self._colour_offset
        self.colour_span = self._colour_span
        for n, colour in enumerate(self._colours):
            self.set_colour(n, colour)

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
        return self._width_fraction * self._img_width

    @plot_width.setter
    def plot_width(self, value):
        self.width_fraction = value / self._img_width

    @property
    def plot_height(self):
        return self._height_fraction * self._img_height

    @plot_height.setter
    def plot_height(self, value):
        self.height_fraction = value / self._img_height

    @property
    def width_fraction(self):
        return self._width_fraction

    @width_fraction.setter
    def width_fraction(self, value):
        self._width_fraction = value
        if self._shell:
            self._shell.send("mandelbrot.set_width_fraction(%.15f)\n" % self._width_fraction)

    @property
    def height_fraction(self):
        return self._height_fraction

    @height_fraction.setter
    def height_fraction(self, value):
        self._height_fraction = value
        if self._shell:
            self._shell.send("mandelbrot.set_height_fraction(%.15f)\n" % self._height_fraction)

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
    def colour_offset(self):
        return self._colour_offset

    @colour_offset.setter
    def colour_offset(self, value):
        self._colour_offset = value
        if self._shell:
            self._shell.send("mandelbrot.set_colour_offset(%i)\n" % self._colour_offset)

    @property
    def colour_span(self):
        return self._colour_span

    @colour_span.setter
    def colour_span(self, value):
        self._colour_span = value
        if self._shell:
            self._shell.send("mandelbrot.set_colour_span(%i)\n" % self._colour_span)

    @property
    def colours(self):
        return tuple(self._colours)

    def get_colour(self, n):
        return self._colours[n]

    def set_colour(self, n, value):
        self._colours[n] = value
        if self._shell:
            self._shell.send("mandelbrot.set_colour_%i(%i)\n" % (n, self._colours[n]))

    def calculate(self):
        if self.shell:
            self._shell.send("renderer.render_to_hdmi()\n")
        return Mandelbrot.calculate(
            self._img_width,
            self._img_height,
            self._width_fraction,
            self._height_fraction,
            self._plot_x_min,
            self._plot_y_max,
            self._max_iteration,
            self._colour_offset,
            self._colour_span,
            self._colours
        )


