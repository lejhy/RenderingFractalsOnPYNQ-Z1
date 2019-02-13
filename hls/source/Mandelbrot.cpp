#include "Mandelbrot.h"

void mandelbrot(AXI_STREAM& OUTPUT_STREAM, Config& config) {
	#pragma HLS INTERFACE s_axilite register port=config
	#pragma HLS INTERFACE axis port=OUTPUT_STREAM
	#pragma HLS INTERFACE s_axilite port=return
	#pragma HLS DATAFLOW

	RGB_IMAGE img(config.img_height, config.img_width);
	calculate(config, img);
	hls::Mat2AXIvideo(img, OUTPUT_STREAM);

	return;
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
	int_16 iter[PARALLEL_LOOPS][PARALLEL_SUB_LOOPS][SUB_LOOP_WIDTH];
	#pragma HLS array_partition variable=iter complete dim=2
	#pragma HLS array_partition variable=iter complete dim=1

	int_16 max_iter = config.max_iteration;
	int_16 img_height = config.img_height;
	int_16 img_width = config.img_width;
	fixed_32_4_SAT plot_height = config.plot_height;
	fixed_32_4_SAT plot_width = config.plot_width;
	fixed_32_4_SAT plot_x_min = config.plot_x_min;
	fixed_32_4_SAT plot_y_max = config.plot_y_max;
	fixed_32_4_SAT width_fraction = (fixed_32_4_SAT)1 / img_width;
	fixed_32_4_SAT height_fraction = (fixed_32_4_SAT)1 / img_height;

	for(int_16 img_y = 0; img_y <= img_height; img_y = img_y + PARALLEL_LOOPS) {
		// '<=' used since one extra loop is needed for img.write at the end

		for(int_16 i = 0; i < PARALLEL_LOOPS; i++){// MAINTENANCE LOOP
			int_16 img_x_offset = 0;

			for(int_16 j = 0; j < PARALLEL_SUB_LOOPS; j++){
				for(int_16 sub_x = 0; sub_x < SUB_LOOP_WIDTH; sub_x++){
					#pragma HLS LOOP_FLATTEN
					#pragma HLS PIPELINE


					if (img_y != 0) {// IMAGE WRITING CODE
						// nothing to write at beginning of first loop
						img.write(getPixel(iter[i][j][sub_x], max_iter));
					}

					if (img_y != img_height) {// INITIALISATION CODE
						// nothing to initialise during the extra loop

						int_16 img_x = img_x_offset + sub_x;
						fixed_32_4_SAT x_0_t1 = img_x * width_fraction;
						fixed_32_4_SAT x_0_t2 = plot_width * x_0_t1;

						x_0[i][j][sub_x] = x_0_t2 + plot_x_min;

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

			for(int_16 i = 0; i < PARALLEL_LOOPS; i++) {// PROCESSING LOOP
				#pragma HLS UNROLL
				for (int_16 j = 0; j < PARALLEL_SUB_LOOPS; j++) {
					#pragma HLS UNROLL
					subLineProcess(x_0[i][j], y_0[i][j], x[i][j], y[i][j], iter[i][j], max_iter);
				}
			}
		}

	}
}

void subLineProcess(fixed_32_4_SAT x_0[], fixed_32_4_SAT y_0[], fixed_32_4_SAT x[], fixed_32_4_SAT y[], int_16 iter[], int_16 max_iter){
	for (int j = 0; j < max_iter; j++) {
		for (int_16 sub_x = 0; sub_x < SUB_LOOP_WIDTH; sub_x++) {
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

RGB_PIXEL getPixel(int_16& iteration, int_16& max_iteration) {
	#pragma HLS INLINE
	RGB_PIXEL result;
	if (iteration < max_iteration) {
		int colour = iteration % 512;
		if (colour > 255) {
			colour = colour - 256;
			result = RGB_PIXEL(colour, 255, colour);
		} else{
			result = RGB_PIXEL(0, colour, 0);
		}
	} else {
		result = RGB_PIXEL(0,0,0);
	}
	return result;
}
