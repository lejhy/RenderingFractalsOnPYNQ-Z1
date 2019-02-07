from model.Mandelbrot_SW import Mandelbrot
from view.MatplotlibView import MatplotlibView
from controller.Controller import DesktopController

model = Mandelbrot()
view = MatplotlibView(1920, 1080)
controller = DesktopController(model, view)
