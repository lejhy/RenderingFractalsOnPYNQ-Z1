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
	RGB_PIXEL pixel_buffer[PARALLEL_LOOPS];
	#pragma HLS array_partition variable=pixel_buffer complete



	// image coordinates have origin at top left, plot coordinates have origin at bottom left
	for(int img_y = 0; img_y < config.img_height; img_y++) {
		for(int img_x = 0; img_x < config.img_width; img_x = img_x + PARALLEL_LOOPS) {
			batchProcess(pixel_buffer, img_x, img_y, config);
			for(int i = 0; i < PARALLEL_LOOPS; i++){
				img.write(pixel_buffer[i]);
			}
		}
	}
}

void batchProcess(RGB_PIXEL pixel_buffer[PARALLEL_LOOPS], int img_x, int img_y, Config config){
	int max_iter = config.max_iteration;
	float img_height = (float)config.img_height;
	float img_width = (float)config.img_width;
	float plot_height = config.plot_height;
	float plot_width = config.plot_width;
	float plot_x_min = config.plot_x_min;
	float plot_y_max = config.plot_y_max;
	float plot_y = plot_y_max - img_y / img_height * plot_height;

	for(int i = 0; i < PARALLEL_LOOPS; i++){
		#pragma HLS UNROLL
		float plot_x = (img_x + i) / img_width * plot_width + plot_x_min;
		int iteration = getIteration(plot_x, plot_y, max_iter);
		pixel_buffer[i] = getPixel(iteration, max_iter);
	}
}

int getIteration(float plot_x, float plot_y, int max_iter) {
	float x = 0.0;
	float y = 0.0;
	int iteration = 0;
	for (int j = 0; j < max_iter; j++) {
		float temp = x*x - y*y + plot_x;
		y = 2*x*y + plot_y;
		x = temp;
		iteration = iteration + (x*x + y*y < 4 && iteration < max_iter);
	}
	return iteration;
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
