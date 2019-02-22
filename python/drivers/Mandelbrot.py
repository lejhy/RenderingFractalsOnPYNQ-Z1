from pynq import DefaultIP
import time
import PIL.Image
import asyncio
import struct

class Mandelbrot(DefaultIP):
    def __init__(self, description):
        super().__init__(description=description)
        # Set up resolution
        self.write(0x10,1920)
        self.write(0x18,1080)
        # Enable interrupts for ap_done
        self.write(0x04,1)
        self.write(0x08,1)
        # Setup default config
        self.config(3.5, 2.0, -2.5, 1.0, 100)

    bindto = ['xilinx.com:hls:mandelbrot:1.0']

    def config(self, plot_width, plot_height, x_min, y_max, iterations):
        self.write(0x20,struct.pack('f', plot_width))
        self.write(0x28,struct.pack('f', plot_height))
        self.write(0x30,struct.pack('f', x_min))
        self.write(0x38,struct.pack('f', y_max))
        self.write(0x40,iterations)

    def set_plot_width(self, plot_width):
        self.write(0x20,struct.pack('f', plot_width))

    def set_plot_height(self, plot_height):
        self.write(0x28,struct.pack('f', plot_height))

    def set_x_min(self, x_min):
        self.write(0x30,struct.pack('f', x_min))

    def set_y_max(self, y_max):
        self.write(0x38,struct.pack('f', y_max))

    def set_iterations(self, iterations):
        self.write(0x40,iterations)

    def get_render(self):
        self.render()
        # Get output
        activeframe = vdma.readchannel.activeframe
        frame = vdma.readchannel._frames[activeframe]
        return PIL.Image.fromarray(frame)

    def timed_get_render(self):
        start = time.time()
        render = self.get_render()
        end = time.time()
        print("Render took:", end - start, "s")
        return render

    def render(self):
        # Start IP
        self.write(0x00,1)
        # Wait for interrupt
        loop = asyncio.get_event_loop()
        while(self.read(0x0c) & 0x1 == 0):
            loop.run_until_complete(asyncio.ensure_future(self.interrupt.wait()))
        # Clear interrupt
        self.write(0x0c, 0x1)
        self.interrupt.event.clear()