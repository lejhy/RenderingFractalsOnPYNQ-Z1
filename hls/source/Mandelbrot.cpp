#include "Mandelbrot.h"

void calc_pixel(int plot_x, int plot_y, int* iteration, Config& config) {

	double x_scaled = (double)plot_x / (double)config.plot_width * config.scale_width + config.scale_x_min;
	double y_scaled = (double)plot_y / (double)config.plot_height * config.scale_height + config.scale_y_min;
	double x = 0.0;
	double y = 0.0;
	*iteration = 0;
	while (x*x + y*y < 4 && *iteration < config.max_iteration) {
		double temp = x*x - y*y + x_scaled;
		y = 2*x*y + y_scaled;
		x = temp;
		*iteration = *iteration + 1;
	}
}
