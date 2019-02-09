#include <iostream>
#include "Mandelbrot.h"
#include "Config.h"

using namespace std;

int main () {
	Config config;
	set_config (1920, 1080, -2.5, 1.0, -1.0, 1.0, 1000, config);
	int iteration;
	calc_pixel(0, 0, &iteration, config);

	cout << iteration << endl;

	calc_pixel(960, 540, &iteration, config);

	cout << iteration << endl;
	return 0;
}
