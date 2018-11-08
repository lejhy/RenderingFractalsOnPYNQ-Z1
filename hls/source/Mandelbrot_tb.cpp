#include <iostream>
#include <hls_opencv.h>
#include "Mandelbrot.h"
#include "Config.h"

using namespace std;

int main () {
	Config config;
	config.img_width = 1920;
	config.img_height = 1080;
	config.plot_x_min = -2.5;
	config.plot_y_max = 0.0;
	config.plot_width = 3.5;
	config.plot_height = 2.0;
	config.max_iteration = 1000;

	AXI_STREAM img_axi;
	IplImage* img = cvCreateImage(cvSize(config.img_width, config.img_height), 8, 3);

	mandelbrot(img_axi, config);

	AXIvideo2IplImage(img_axi, img);

	cvSaveImage("img.bmp", img);

	return 0;
}
