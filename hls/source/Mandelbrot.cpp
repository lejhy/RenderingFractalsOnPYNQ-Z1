#include "Mandelbrot.h"

void mandelbrot(AXI_STREAM& OUTPUT_STREAM, Config& config) {
	#pragma HLS INTERFACE s_axilite port=config
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
	#pragma HLS INLINE region
	#pragma HLS DATAFLOW

	for(int i = 0; i < PARALLEL_LOOPS; i++){
		#pragma HLS UNROLL
		float plot_x = (float)(img_x + i) / (float)config.img_width * config.plot_width + config.plot_x_min;
		float plot_y = config.plot_y_max - img_y / (float)config.img_height * config.plot_height;
		float x = 0.0;
		float y = 0.0;
		int iteration = 0;
		while (x*x + y*y < 4 && iteration < config.max_iteration) {
			float temp = x*x - y*y + plot_x;
			y = 2*x*y + plot_y;
			x = temp;
			iteration = iteration + 1;
		}
		pixel_buffer[i] = getPixel(iteration, config.max_iteration);
	}
}

RGB_PIXEL getPixel(int& iteration, int& max_iteration) {
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
