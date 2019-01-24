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
	fixed_32_4_SAT x_0[PARALLEL_LOOPS][WIDTH];
	#pragma HLS array_partition variable=x_0 complete dim=1
	fixed_32_4_SAT y_0[PARALLEL_LOOPS][WIDTH];
	#pragma HLS array_partition variable=y_0 complete dim=1
	fixed_32_4_SAT x[PARALLEL_LOOPS][WIDTH];
	#pragma HLS array_partition variable=x complete dim=1
	fixed_32_4_SAT y[PARALLEL_LOOPS][WIDTH];
	#pragma HLS array_partition variable=y complete dim=1
	int_16 iter[PARALLEL_LOOPS][WIDTH];
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


	// image coordinates have origin at top left, plot coordinates have origin at bottom left
	for(int_16 img_y = 0; img_y < config.img_height; img_y = img_y + PARALLEL_LOOPS) {

		for(int_16 i = 0; i < PARALLEL_LOOPS; i++) {
			#pragma HLS UNROLL
			lineProcess(x_0[i], y_0[i], x[i], y[i], iter[i], img_y+i, max_iter, img_height, img_width, plot_height, plot_width, plot_x_min, plot_y_max, width_fraction, height_fraction);
		}

		for(int_16 i = 0; i < PARALLEL_LOOPS; i++){
			for(int_16 img_x = 0; img_x < WIDTH; img_x++){
				#pragma HLS LOOP_FLATTEN
				img.write(getPixel(iter[i][img_x], max_iter));
			}
		}
	}
}

void lineProcess(fixed_32_4_SAT x_0[], fixed_32_4_SAT y_0[], fixed_32_4_SAT x[], fixed_32_4_SAT y[], int_16 iter[], int_16 img_y, int_16 max_iter, int_16 img_height, int_16 img_width, fixed_32_4_SAT plot_height, fixed_32_4_SAT plot_width, fixed_32_4_SAT plot_x_min, fixed_32_4_SAT plot_y_max, fixed_32_4_SAT width_fraction, fixed_32_4_SAT height_fraction){

	for(int_16 img_x = 0; img_x < WIDTH; img_x++) {
		#pragma HLS PIPELINE
		fixed_32_4_SAT x_0_t1 = img_x * width_fraction;
		fixed_32_4_SAT x_0_t2 = plot_width * x_0_t1;

		x_0[img_x] = x_0_t2 + plot_x_min;

		fixed_32_4_SAT y_0_t1 = img_y * height_fraction;
		fixed_32_4_SAT y_0_t2 = plot_height * y_0_t1;

		y_0[img_x] = plot_y_max - y_0_t2;

		x[img_x] = 0;
		y[img_x] = 0;
		iter[img_x] = 0;
	}

	for (int j = 0; j < max_iter; j++) {
		for (int_16 img_x = 0; img_x < WIDTH; img_x++) {
			#pragma HLS PIPELINE
			fixed_32_4_SAT x_sq = x[img_x]*x[img_x];
			fixed_32_4_SAT y_sq = y[img_x]*y[img_x];

			fixed_32_4_SAT temp = x_sq - y_sq + x_0[img_x];
			fixed_32_4_SAT x_y = x[img_x]*y[img_x];
			fixed_32_4_SAT x_y_2 = x_y * 2;

			y[img_x] = x_y_2 + y_0[img_x];
			x[img_x] = temp;

			iter[img_x] = iter[img_x] + (x_sq + y_sq < 4);
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
