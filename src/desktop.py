import time

from model.Mandelbrot import Mandelbrot
from view.MatplotlibView import MatplotlibView
from controller.DesktopController import DesktopController

model = Mandelbrot()
view = MatplotlibView(1920, 1080)
controller = DesktopController(model, view)

start: float = time.time()
controller.update(x_scale=[-2.5, 1], y_scale=[-1, 1], max_iteration=1000)
end = time.time()
print("Render took:", end - start, "s")