import time

from model.Mandelbrot_SW import Mandelbrot
from view.MatplotlibView import MatplotlibView
from controller.Controller import Controller

model = Mandelbrot()
view = MatplotlibView(1920, 1080)
controller = Controller(model, view)

start: float = time.time()
controller.update(x_scale=[-2.5, 1], y_scale=[-1, 1], max_iteration=1000)
end = time.time()
print("Render took:", end - start, "s")