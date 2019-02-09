#include <iostream>
#include "Mandelbrot.h"
#include "Config.h"

using namespace std;

int main () {
  Config config;
  config.img_width = 1920;
  config.img_height = 1080;
  config.plot_x_min = -1.3;
  config.plot_y_max = 0.35;
  config.plot_width = 0.35;
  config.plot_height = 0.2;
  config.max_iteration = 1000;

  int iteration;

  calc_pixel(0, 0, &iteration, config);
  cout << iteration << endl;

  calc_pixel(960, 540, &iteration, config);
  cout << iteration << endl;

  return 0;
}
