#include "Mandelbrot.h"

void mandelbrot(AXI_STREAM& OUTPUT_STREAM, Config& config) {
    #pragma HLS INTERFACE s_axilite register port=config
    #pragma HLS INTERFACE axis port=OUTPUT_STREAM
    #pragma HLS INTERFACE s_axilite port=return
    #pragma HLS DATAFLOW

    RGB_IMAGE img(config.img_height, config.img_width);
    calculate(config, img);
    hls::Mat2AXIvideo(img, OUTPUT_STREAM);
}

void calculate(Config& config, RGB_IMAGE& img) {

    fixed_32_4_SAT x_0[PARALLEL_LOOPS][PARALLEL_SUB_LOOPS][SUB_LOOP_WIDTH];
    #pragma HLS ARRAY_PARTITION variable=x_0 complete dim=2
    #pragma HLS array_partition variable=x_0 complete dim=1
    fixed_32_4_SAT y_0[PARALLEL_LOOPS][PARALLEL_SUB_LOOPS][SUB_LOOP_WIDTH];
    #pragma HLS array_partition variable=y_0 complete dim=2
    #pragma HLS array_partition variable=y_0 complete dim=1
    fixed_32_4_SAT x[PARALLEL_LOOPS][PARALLEL_SUB_LOOPS][SUB_LOOP_WIDTH];
    #pragma HLS array_partition variable=x complete dim=2
    #pragma HLS array_partition variable=x complete dim=1
    fixed_32_4_SAT y[PARALLEL_LOOPS][PARALLEL_SUB_LOOPS][SUB_LOOP_WIDTH];
    #pragma HLS array_partition variable=y complete dim=2
    #pragma HLS array_partition variable=y complete dim=1
    uint16_t iter[PARALLEL_LOOPS][PARALLEL_SUB_LOOPS][SUB_LOOP_WIDTH];
    #pragma HLS array_partition variable=iter complete dim=2
    #pragma HLS array_partition variable=iter complete dim=1

    uint16_t max_iter = config.max_iteration;
    uint16_t img_height = config.img_height;
    uint16_t img_width = config.img_width;
    fixed_32_4_SAT plot_height = config.plot_height;
    fixed_32_4_SAT plot_width = config.plot_width;
    fixed_32_4_SAT plot_x_min = config.plot_x_min;
    fixed_32_4_SAT plot_y_max = config.plot_y_max;
    fixed_32_4_SAT width_fraction = (fixed_32_4_SAT)1 / img_width;
    fixed_32_4_SAT height_fraction = (fixed_32_4_SAT)1 / img_height;

    uint16_t colour_offset = config.colour_offset;
    uchar colour_span = config.colour_span;
    uchar colour_0_r = config.colour_0 >> 16;
    uchar colour_0_g = config.colour_0 >> 8;
    uchar colour_0_b = config.colour_0;
    uchar colour_1_r = config.colour_1 >> 16;
    uchar colour_1_g = config.colour_1 >> 8;
    uchar colour_1_b = config.colour_1;
    uchar colour_2_r = config.colour_2 >> 16;
    uchar colour_2_g = config.colour_2 >> 8;
    uchar colour_2_b = config.colour_2;
    uchar colour_3_r = config.colour_3 >> 16;
    uchar colour_3_g = config.colour_3 >> 8;
    uchar colour_3_b = config.colour_3;
    uchar colour_4_r = config.colour_4 >> 16;
    uchar colour_4_g = config.colour_4 >> 8;
    uchar colour_4_b = config.colour_4;

    for(uint16_t img_y = 0; img_y <= img_height; img_y = img_y + PARALLEL_LOOPS) {
        // '<=' used since one extra loop is needed for img.write at the end

        for(uint16_t i = 0; i < PARALLEL_LOOPS; i++){// MAINTENANCE LOOP
            uint16_t img_x_offset = 0;

            for(uint16_t j = 0; j < PARALLEL_SUB_LOOPS; j++){
                for(uint16_t sub_x = 0; sub_x < SUB_LOOP_WIDTH; sub_x++){
                    #pragma HLS LOOP_FLATTEN
                    #pragma HLS PIPELINE

                    if (img_y != 0) {// IMAGE WRITING CODE
                        // nothing to write at beginning of first loop
                        img.write(getPixel(
                                iter[i][j][sub_x],
                                max_iter,
                                colour_offset,
                                colour_span,
                                colour_0_r,
                                colour_0_g,
                                colour_0_b,
                                colour_1_r,
                                colour_1_g,
                                colour_1_b,
                                colour_2_r,
                                colour_2_g,
                                colour_2_b,
                                colour_3_r,
                                colour_3_g,
                                colour_3_b,
                                colour_4_r,
                                colour_4_g,
                                colour_4_b
                        ));
                    }

                    if (img_y != img_height) {// INITIALISATION CODE
                        // nothing to initialise during the extra loop

                        uint16_t img_x = img_x_offset + sub_x;
                        fixed_32_4_SAT x_0_t1 = img_x * width_fraction;
                        fixed_32_4_SAT x_0_t2 = plot_width * x_0_t1;
                        x_0[i][j][sub_x] = plot_x_min + x_0_t2;

                        // use current img_y pixel row number (img_y+i)
                        fixed_32_4_SAT y_0_t1 = (img_y+i) * height_fraction;
                        fixed_32_4_SAT y_0_t2 = plot_height * y_0_t1;
                        y_0[i][j][sub_x] = plot_y_max - y_0_t2;

                        x[i][j][sub_x] = 0;
                        y[i][j][sub_x] = 0;
                        iter[i][j][sub_x] = 0;
                    }
                }
                img_x_offset += SUB_LOOP_WIDTH;
            }
        }

        if (img_y != img_height) {
            // nothing to process during the extra loop

            for(uint16_t i = 0; i < PARALLEL_LOOPS; i++) {// PROCESSING LOOP
                #pragma HLS UNROLL
                for (uint16_t j = 0; j < PARALLEL_SUB_LOOPS; j++) {
                    #pragma HLS UNROLL
                    subLineProcess(
                        x_0[i][j],
                        y_0[i][j],
                        x[i][j],
                        y[i][j],
                        iter[i][j],
                        max_iter
                    );
                }
            }
        }

    }
}

void subLineProcess(
    fixed_32_4_SAT x_0[],
    fixed_32_4_SAT y_0[],
    fixed_32_4_SAT x[],
    fixed_32_4_SAT y[],
    uint16_t iter[],
    uint16_t max_iter
){
    for (int j = 0; j < max_iter; j++) {
        for (uint16_t sub_x = 0; sub_x < SUB_LOOP_WIDTH; sub_x++) {
            #pragma HLS PIPELINE
            fixed_32_4_SAT x_sq = x[sub_x]*x[sub_x];
            fixed_32_4_SAT y_sq = y[sub_x]*y[sub_x];

            fixed_32_4_SAT temp = x_sq - y_sq + x_0[sub_x];
            fixed_32_4_SAT x_y = x[sub_x]*y[sub_x];
            fixed_32_4_SAT x_y_2 = x_y * 2;

            y[sub_x] = x_y_2 + y_0[sub_x];
            x[sub_x] = temp;

            iter[sub_x] = iter[sub_x] + (x_sq + y_sq < 4);
        }
    }
}

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
) {
    #pragma HLS INLINE

    uchar intensity = 0;
    uchar colour_a_r = 0;
    uchar colour_a_g = 0;
    uchar colour_a_b = 0;
    uchar colour_b_r = 0;
    uchar colour_b_g = 0;
    uchar colour_b_b = 0;

    if (iteration < max_iteration) {
        iteration = iteration < colour_offset ? 0 : iteration - colour_offset;
        if (colour_span < 8) {
            iteration = iteration << (8 - colour_span);
        } else {
            iteration = iteration >> (colour_span - 8);
        }
        intensity = iteration;
        
        if (iteration < 256) {
            colour_a_r = colour_0_r;
            colour_a_g = colour_0_g;
            colour_a_b = colour_0_b;
            colour_b_r = colour_1_r;
            colour_b_g = colour_1_g;
            colour_b_b = colour_1_b;
        } else if (iteration < 512){
            colour_a_r = colour_1_r;
            colour_a_g = colour_1_g;
            colour_a_b = colour_1_b;
            colour_b_r = colour_2_r;
            colour_b_g = colour_2_g;
            colour_b_b = colour_2_b;
        } else if (iteration < 768){
            colour_a_r = colour_2_r;
            colour_a_g = colour_2_g;
            colour_a_b = colour_2_b;
            colour_b_r = colour_3_r;
            colour_b_g = colour_3_g;
            colour_b_b = colour_3_b;
        } else {
            colour_a_r = colour_3_r;
            colour_a_g = colour_3_g;
            colour_a_b = colour_3_b;
            colour_b_r = colour_3_r;
            colour_b_g = colour_3_g;
            colour_b_b = colour_3_b;
        }
    } else {
        colour_a_r = colour_4_r;
        colour_a_g = colour_4_g;
        colour_a_b = colour_4_b;
        colour_b_r = colour_4_r;
        colour_b_g = colour_4_g;
        colour_b_b = colour_4_b;
    }
    uint8_t r = colour_a_r + (((colour_b_r - colour_a_r) * intensity) >> 8);
    uint8_t g = colour_a_g + (((colour_b_g - colour_a_g) * intensity) >> 8);
    uint8_t b = colour_a_b + (((colour_b_b - colour_a_b) * intensity) >> 8);
    return RGB_PIXEL(r, g, b);
}
