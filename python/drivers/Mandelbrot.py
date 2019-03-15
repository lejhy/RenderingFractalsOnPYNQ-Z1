from pynq import DefaultIP

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
        self.set_plot_width(3.5)
        self.set_plot_height(2.0)
        self.set_plot_x_min(-2.5)
        self.set_plot_y_max(1.0)
        self.set_max_iteration(1000)
        self.set_colour_offset(0)
        self.set_colour_span(8)
        self.set_colour_0(0xFFFFFF)
        self.set_colour_1(0x888888)
        self.set_colour_2(0x000000)
        self.set_colour_3(0xFFFFFF)
        self.set_colour_4(0xFFFFFF)

    bindto = ['xilinx.com:hls:mandelbrot:1.0']

    def set_plot_width(self, plot_width):
        self.write(0x20,struct.pack('d', plot_width))

    def set_plot_height(self, plot_height):
        self.write(0x2c,struct.pack('d', plot_height))

    def set_plot_x_min(self, plot_x_min):
        self.write(0x38,struct.pack('d', plot_x_min))

    def set_plot_y_max(self, plot_y_max):
        self.write(0x44,struct.pack('d', plot_y_max))

    def set_max_iteration(self, max_iteration):
        self.write(0x50,max_iteration)

    def set_colour_offset(self, colour_offset):
        self.write(0x58,colour_offset)
        
    def set_colour_span(self, colour_span):
        self.write(0x60,colour_span)

    def set_colour_0(self, colour_0):
        self.write(0x68,colour_0)

    def set_colour_1(self, colour_1):
        self.write(0x70,colour_1)

    def set_colour_2(self, colour_2):
        self.write(0x78,colour_2)

    def set_colour_3(self, colour_3):
        self.write(0x80,colour_3)
        
    def set_colour_4(self, colour_4):
        self.write(0x88,colour_4)

    def calculate(self):
        # Start IP
        self.write(0x00,1)
        # Wait for interrupt
        loop = asyncio.get_event_loop()
        while(self.read(0x0c) & 0x1 == 0):
            loop.run_until_complete(asyncio.ensure_future(self.interrupt.wait()))
        # Clear interrupt
        self.write(0x0c, 0x1)
        self.interrupt.event.clear()

