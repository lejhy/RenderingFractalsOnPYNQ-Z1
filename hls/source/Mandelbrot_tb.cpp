#include <iostream>
#include <hls_opencv.h>
#include "Mandelbrot.h"
#include "Config.h"

using namespace std;

int main () {
    Config config;
    config.img_width = WIDTH;
    config.img_height = HEIGHT;
    config.plot_x_min = -1.3;
    config.plot_y_max = 0.35;
    config.width_fraction = 1.0/WIDTH * 0.35;
    config.height_fraction = 1.0/HEIGHT * 0.2;
    config.max_iteration = 1000;
    config.colour_span = 5;
    config.colour_0 = 0x00ffb444;
    config.colour_1 = 0x007e00af;
    config.colour_2 = 0x00200042;
    config.colour_3 = 0x00FFFFFF;

    AXI_STREAM img_axi;
    IplImage* img = cvCreateImage(cvSize(config.img_width, config.img_height), 8, 3);

    mandelbrot(img_axi, config);

    AXIvideo2IplImage(img_axi, img);

    cvSaveImage("img.bmp", img);

    return 0;
}
