from model.Mandelbrot import Mandelbrot
from view.MatplotlibView import MatplotlibView
from controller.DesktopController import DesktopController

model = Mandelbrot()
view = MatplotlibView(1920, 1080)
controller = DesktopController(model, view)
