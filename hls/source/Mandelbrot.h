#ifndef _MANDELBROT_H_
#define _MANDELBROT_H_

#include "Config.h"
#include "hls_video.h"
#include "ap_fixed.h"

#define PARALLEL_LOOPS 		8
#define PARALLEL_SUB_LOOPS 	2
#define WIDTH 			1920
#define HEIGHT			1080

typedef hls::stream<ap_axiu<32,1,1,1> >     AXI_STREAM;
typedef hls::Scalar<3, unsigned char>       RGB_PIXEL;
typedef hls::Mat<HEIGHT, WIDTH, HLS_8UC3>      RGB_IMAGE;

typedef ap_fixed<32, 4, AP_TRN, AP_SAT> fixed_32_4_SAT;
typedef ap_uint<16> int_16;

void mandelbrot(AXI_STREAM& OUTPUT_STREAM, Config& config);
void calculate(Config& config, RGB_IMAGE& img);
void lineProcess(fixed_32_4_SAT x_0[], fixed_32_4_SAT y_0[], fixed_32_4_SAT x[], fixed_32_4_SAT y[], int_16 iter[], int_16 img_y, int_16 max_iter, int_16 img_height, int_16 img_width, fixed_32_4_SAT plot_height, fixed_32_4_SAT plot_width, fixed_32_4_SAT plot_x_min, fixed_32_4_SAT plot_y_max, fixed_32_4_SAT width_fraction, fixed_32_4_SAT height_fraction);
RGB_PIXEL getPixel(int_16& iteration, int_16& max_iteration);

#endif
