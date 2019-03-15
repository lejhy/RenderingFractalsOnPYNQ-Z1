#ifndef _MANDELBROT_H_
#define _MANDELBROT_H_

#include "Config.h"
#include "hls_video.h"
#include "ap_fixed.h"

#define PARALLEL_LOOPS         8
#define PARALLEL_SUB_LOOPS     2
#define WIDTH                  1920
#define HEIGHT                 1080
#define SUB_LOOP_WIDTH         WIDTH / PARALLEL_SUB_LOOPS

typedef hls::stream<ap_axiu<32,1,1,1>>       AXI_STREAM;
typedef hls::Scalar<3, unsigned char>        RGB_PIXEL;
typedef hls::Mat<HEIGHT, WIDTH, HLS_8UC3>    RGB_IMAGE;

typedef ap_fixed<32, 4, AP_RND, AP_SAT> fixed_32_4_SAT;

void mandelbrot(AXI_STREAM& OUTPUT_STREAM, Config& config);
void calculate(Config& config, RGB_IMAGE& img);
void subLineProcess(
    fixed_32_4_SAT x_0[],
    fixed_32_4_SAT y_0[],
    fixed_32_4_SAT x[],
    fixed_32_4_SAT y[],
    uint16_t iter[],
    uint16_t max_iter
);
RGB_PIXEL getPixel(uint16_t& iteration, uint16_t& max_iteration);

#endif
