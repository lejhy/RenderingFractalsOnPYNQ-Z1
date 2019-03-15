#include <iostream>
#include "Mandelbrot.h"
#include "Config.h"

using namespace std;

int main () {
  Config config;
  config.img_width = 1920;
  config.img_height = 1080;
  config.plot_x_min = -2.5;
  config.plot_y_max = 1.0;
  config.plot_width = 3.5;
  config.plot_height = 2.0;
  config.max_iteration = 1000;

  int iteration;

  calc_pixel(0, 0, &iteration, config);
  cout << iteration << endl;

  calc_pixel(960, 540, &iteration, config);
  cout << iteration << endl;

  return 0;
}
