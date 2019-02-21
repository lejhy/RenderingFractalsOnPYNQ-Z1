#define BOOST_PYTHON_STATIC_LIB
#define BOOST_NUMPY_STATIC_LIB

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace bp = boost::python;
namespace np = boost::python::numpy;

np::ndarray calculate(int img_width, int img_height, double plot_width, double plot_height, double plot_x_min, double plot_y_max, int max_iteration) {
	
	Py_intptr_t shape[3] = { img_height, img_width, 3 };
	np::ndarray img = np::empty(3, shape, np::dtype::get_builtin<uint8_t>());
	char *data = img.get_data();

	double width_fraction = 1.0 / img_width * plot_width;
	double height_fraction = 1.0 / img_height * plot_height;


	for (int img_y = 0; img_y < img_height; img_y++) {
		for (int img_x = 0; img_x < img_width; img_x++) {

			double plot_x = img_x * width_fraction  + plot_x_min;
			double plot_y = plot_y_max - img_y * height_fraction;

			double x = 0.0;
			double y = 0.0;
			int iteration = 0;

			double x_sq = 0;
			double y_sq = 0;

			while (x_sq + y_sq < 4 && iteration < max_iteration) {
				
				y = 2 * x*y + plot_y;
				x = x_sq - y_sq + plot_x;
				
				x_sq = x * x;
				y_sq = y * y;

				iteration++;
			}
			if (iteration < max_iteration) {
				int colour = iteration % 512;
				if (colour > 255) {
					colour = colour - 256;
					data[img_y * img_width * 3 + img_x * 3] = colour;
					data[img_y * img_width * 3 + img_x * 3 + 1] = 0xFF;
					data[img_y * img_width * 3 + img_x * 3 + 2] = colour;
				}
				else {
					data[img_y * img_width * 3 + img_x * 3] = 0x00;
					data[img_y * img_width * 3 + img_x * 3 + 1] = colour;
					data[img_y * img_width * 3 + img_x * 3 + 2] = 0x00;
				}
			}
			else {
				data[img_y * img_width * 3 + img_x * 3] = 0x00;
				data[img_y * img_width * 3 + img_x * 3 + 1] = 0x00;
				data[img_y * img_width * 3 + img_x * 3 + 2] = 0x00;
			}
		}
	}

	return img;
}



BOOST_PYTHON_MODULE(Mandelbrot) {
	np::initialize();
	bp::def("calculate", calculate);
}