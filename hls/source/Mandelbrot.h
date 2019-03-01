#ifndef _MANDELBROT_H_
#define _MANDELBROT_H_

#include "Config.h"
#include "hls_video.h"
#include "ap_fixed.h"
#include "ap_int.h"

#define PARALLEL_LOOPS 		8
#define PARALLEL_SUB_LOOPS 	2
#define WIDTH 				1920
#define HEIGHT				1080
#define SUB_LOOP_WIDTH 		WIDTH / PARALLEL_SUB_LOOPS

typedef unsigned char						uchar;
typedef hls::stream<ap_axiu<32,1,1,1> >     AXI_STREAM;
typedef hls::Scalar<3, uchar>       RGB_PIXEL;
typedef hls::Mat<HEIGHT, WIDTH, HLS_8UC3>   RGB_IMAGE;

typedef ap_fixed<32, 4, AP_RND, AP_SAT> fixed_32_4_SAT;

void mandelbrot(AXI_STREAM& OUTPUT_STREAM, Config& config);
void calculate(Config& config, RGB_IMAGE& img);
void subLineProcess(fixed_32_4_SAT x_0[], fixed_32_4_SAT y_0[], fixed_32_4_SAT x[], fixed_32_4_SAT y[], uint16_t iter[], uint16_t max_iter);
RGB_PIXEL getPixel(
        uint16_t& iteration,
        uint16_t& max_iteration,
        uint16_t& colour_offset,
        uchar& colour_span,
        uchar& colour_0_r,
        uchar& colour_0_g,
        uchar& colour_0_b,
        uchar& colour_1_r,
        uchar& colour_1_g,
        uchar& colour_1_b,
        uchar& colour_2_r,
        uchar& colour_2_g,
        uchar& colour_2_b,
        uchar& colour_3_r,
        uchar& colour_3_g,
        uchar& colour_3_b,
        uchar& colour_4_r,
        uchar& colour_4_g,
        uchar& colour_4_b
);

#endif
