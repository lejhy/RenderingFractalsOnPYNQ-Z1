#include "Mandelbrot.h"

void calc_pixel(int img_x, int img_y, int* iteration, Config& config) {
  #pragma HLS INTERFACE s_axilite port=config
  #pragma HLS INTERFACE s_axilite port=iteration
  #pragma HLS INTERFACE s_axilite port=img_y
  #pragma HLS INTERFACE s_axilite port=img_x
  #pragma HLS INTERFACE s_axilite port=return

  double x_scaled = (double)img_x / (double)config.img_width * config.plot_width + config.plot_x_min;
  double y_scaled = config.plot_y_max - (double)img_y / (double)config.img_height * config.plot_height;
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
