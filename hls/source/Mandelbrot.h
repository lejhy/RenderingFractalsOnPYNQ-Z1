#ifndef _MANDELBROT_H_
#define _MANDELBROT_H_

#include "Config.h"
#include "hls_video.h"

#define PARALLEL_LOOPS 8

typedef hls::stream<ap_axiu<32,1,1,1> >     AXI_STREAM;
typedef hls::Scalar<3, unsigned char>       RGB_PIXEL;
typedef hls::Mat<1080, 1920, HLS_8UC3>      RGB_IMAGE;

void mandelbrot(AXI_STREAM& OUTPUT_STREAM, Config& config);
void calculate(Config& config, RGB_IMAGE& img);
void lineProcess(float x_0[], float y_0[], float x[], float y[], int iter[], int img_y, int max_iter, float img_height, float img_width, float plot_height, float plot_width, float plot_x_min, float plot_y_max);
RGB_PIXEL getPixel(int& iteration, int& max_iteration);

#endif
