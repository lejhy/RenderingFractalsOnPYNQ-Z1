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
	float x_0[PARALLEL_LOOPS][1920];
	#pragma HLS array_partition variable=x_0 complete dim=1
	float y_0[PARALLEL_LOOPS][1920];
	#pragma HLS array_partition variable=y_0 complete dim=1
	float x[PARALLEL_LOOPS][1920];
	#pragma HLS array_partition variable=x complete dim=1
	float y[PARALLEL_LOOPS][1920];
	#pragma HLS array_partition variable=y complete dim=1
	int iter[PARALLEL_LOOPS][1920];
	#pragma HLS array_partition variable=iter complete dim=1


	int max_iter = config.max_iteration;
	float img_height = (float)config.img_height;
	float img_width = (float)config.img_width;
	float plot_height = config.plot_height;
	float plot_width = config.plot_width;
	float plot_x_min = config.plot_x_min;
	float plot_y_max = config.plot_y_max;


	// image coordinates have origin at top left, plot coordinates have origin at bottom left
	for(int img_y = 0; img_y < config.img_height; img_y = img_y + PARALLEL_LOOPS) {

		for(int i = 0; i < PARALLEL_LOOPS; i++) {
			#pragma HLS UNROLL
			lineProcess(x_0[i], y_0[i], x[i], y[i], iter[i], img_y+i, max_iter, img_height, img_width, plot_height, plot_width, plot_x_min, plot_y_max);
		}

		for(int i = 0; i < PARALLEL_LOOPS; i++){
			for(int img_x = 0; img_x < 1920; img_x++){
				#pragma HLS LOOP_FLATTEN
				img.write(getPixel(iter[i][img_x], max_iter));
			}
		}
	}
}

void lineProcess(float x_0[], float y_0[], float x[], float y[], int iter[], int img_y, int max_iter, float img_height, float img_width, float plot_height, float plot_width, float plot_x_min, float plot_y_max){
	for(int img_x = 0; img_x < 1920; img_x++) {
		#pragma HLS PIPELINE
		x_0[img_x] = img_x / img_width * plot_width + plot_x_min;
		y_0[img_x] = plot_y_max - img_y / img_height * plot_height;
		x[img_x] = 0;
		y[img_x] = 0;
		iter[img_x] = 0;
	}

	for (int j = 0; j < max_iter; j++) {
		for (int img_x = 0; img_x < 1920; img_x++) {
			#pragma HLS PIPELINE
			float temp = x[img_x]*x[img_x] - y[img_x]*y[img_x] + x_0[img_x];
			y[img_x] = 2*x[img_x]*y[img_x] + y_0[img_x];
			x[img_x] = temp;
			iter[img_x] = iter[img_x] + (x[img_x]*x[img_x] + y[img_x]*y[img_x] < 4 && iter[img_x] < max_iter);
		}
	}
}

RGB_PIXEL getPixel(int& iteration, int& max_iteration) {
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
