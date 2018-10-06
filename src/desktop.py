from model.Mandelbrot import Mandelbrot
from view.MatplotlibView import MatplotlibView
from controller.DesktopController import DesktopController

model = Mandelbrot()
view = MatplotlibView(1400, 800)
controller = DesktopController(model, view)
