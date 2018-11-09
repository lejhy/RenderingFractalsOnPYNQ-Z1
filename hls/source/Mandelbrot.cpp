#include "Mandelbrot.h"

void mandelbrot(AXI_STREAM& OUTPUT_STREAM, Config& config) {

	RGB_IMAGE img(config.img_height, config.img_width);

	calculate(config, img);

	hls::Mat2AXIvideo(img, OUTPUT_STREAM);
}

void calculate(Config& config, RGB_IMAGE& img) {
	// image coordinates have origin at top left, plot coordinates have origin at bottom left
	for(int img_y = 0; img_y < 1080; img_y++) {
		for(int img_x = 0; img_x < 1920; img_x++) {
			double plot_x = (double)img_x / (double)config.img_width * config.plot_width + config.plot_x_min;
			double plot_y = config.plot_y_max - (double)img_y / (double)config.img_height * config.plot_height;
			double x = 0.0;
			double y = 0.0;
			int iteration = 0;
			while (x*x + y*y < 4 && iteration < config.max_iteration) {
				double temp = x*x - y*y + plot_x;
				y = 2*x*y + plot_y;
				x = temp;
				iteration = iteration + 1;
			}
			img.write(getPixel(iteration, config.max_iteration));
		}
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
