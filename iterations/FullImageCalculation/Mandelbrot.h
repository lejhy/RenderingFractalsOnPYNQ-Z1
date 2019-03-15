#ifndef _MANDELBROT_H_
#define _MANDELBROT_H_

#include "Config.h"
#include "hls_video.h"

#define WIDTH 				1920
#define HEIGHT				1080

typedef hls::stream<ap_axiu<32,1,1,1> >     AXI_STREAM;
typedef hls::Scalar<3, unsigned char>       RGB_PIXEL;
typedef hls::Mat<HEIGHT, WIDTH, HLS_8UC3>      RGB_IMAGE;

void mandelbrot(AXI_STREAM& OUTPUT_STREAM, Config& config);
void calculate(Config& config, RGB_IMAGE& img);
RGB_PIXEL getPixel(int& iteration, int& max_iteration);

#endif
