import struct

class Mandelbrot:

    def __init__(self, ip):
        self.ip = ip

    def calculate(self, x_scale, y_scale, plot_width, plot_height, max_iteration):
        v_min = 0
        v_max = max_iteration
        iterations = [[0 for _ in range(plot_width)] for _ in range(plot_height)]
        scale_width = x_scale[1] - x_scale[0]
        scale_height = y_scale[1] - y_scale[0]
        self.ip.write(0x28,plot_width)
        self.ip.write(0x30,plot_height)
        self.ip.write(0x38,struct.pack('d', scale_width))
        self.ip.write(0x44,struct.pack('d', scale_height))
        self.ip.write(0x50,struct.pack('d', x_scale[0]))
        self.ip.write(0x5c,struct.pack('d', y_scale[0]))
        self.ip.write(0x68,max_iteration)
        for plot_x in range(plot_width):
            for plot_y in range(plot_height):
                """calculate pixels"""
                self.ip.write(0x10,plot_x)
                self.ip.write(0x18,plot_y)
                self.ip.write(0x00,1)
                iterations[plot_y][plot_x] = self.ip.read(0x20)

        return {
            "values": iterations,
            "vMin": v_min,
            "vMax": v_max
        }
