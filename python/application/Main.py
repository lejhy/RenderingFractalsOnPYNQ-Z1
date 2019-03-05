import time

from python.application.model.Mandelbrot_CPP import Mandelbrot_CPP
from python.application.view.PyQtView import PyQtView
from python.application.controller.Controller import Controller

model = Mandelbrot_CPP()
view = PyQtView(1920, 1080)
controller = Controller(model, view)
