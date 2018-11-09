#include <iostream>
#include <hls_opencv.h>
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

	AXI_STREAM img_axi;
	IplImage* img = cvCreateImage(cvSize(config.img_width, config.img_height), 8, 3);

	mandelbrot(img_axi, config);

	AXIvideo2IplImage(img_axi, img);

	cvSaveImage("img.bmp", img);

	return 0;
}
