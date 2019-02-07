
class Mandelbrot:

    def calculate(self, x_scale, y_scale, plot_width, plot_height, max_iteration):
        v_min = max_iteration
        v_max = 0
        iterations = [[0 for _ in range(plot_width)] for _ in range(plot_height)]
        scale_width = x_scale[1] - x_scale[0]
        scale_height = y_scale[1] - y_scale[0]
        for plot_x in range(plot_width):
            for plot_y in range(plot_height):
                """calculate pixels"""
                x_scaled = plot_x / plot_width * scale_width + x_scale[0]
                y_scaled = plot_y / plot_height * scale_height + y_scale[0]
                x = 0.0
                y = 0.0
                iteration = 0
                while x*x + y*y < 4 and iteration < max_iteration:
                    temp = x*x - y*y + x_scaled
                    y = 2*x*y + y_scaled
                    x = temp
                    iteration = iteration + 1
                iterations[plot_y][plot_x] = iteration
                if iteration < v_min:
                    v_min = iteration
                if iteration > v_max:
                    v_max = iteration

        return {
            "values": iterations,
            "vMin": v_min,
            "vMax": v_max
        }
