import time
import PIL.Image

class Renderer:
    def __init__(self, mandelbrot, vdma, hdmi_out):
        self.mandelbrot = mandelbrot
        self.vdma = vdma
        self.hdmi_out = hdmi_out

    def get_active_frame(self):
        activeframe = self.vdma.readchannel.activeframe
        return self.vdma.readchannel._frames[activeframe]

    def render_to_image(self):
        self.mandelbrot.calculate()
        # Get output
        frame = self.get_active_frame()
        return PIL.Image.fromarray(frame)

    def timed_render_to_image(self):
        start = time.time()
        image = self.render_to_notebook()
        end = time.time()
        print("Render took:", end - start, "s")
        return image

    def render_to_hdmi(self):
        self.mandelbrot.calculate()
        # Stream to HDMI
        frame = self.get_active_frame()
        self.hdmi_out.writeframe(frame)
