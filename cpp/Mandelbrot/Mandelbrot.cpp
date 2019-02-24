#define BOOST_PYTHON_STATIC_LIB
#define BOOST_NUMPY_STATIC_LIB
#define BOOST_COMPUTE_STATIC_LIB

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/compute.hpp>

namespace bp = boost::python;
namespace np = boost::python::numpy;
namespace cp = boost::compute;

np::ndarray calculate(
	int img_width, 
	int img_height, 
	double plot_width, 
	double plot_height, 
	double plot_x_min, 
	double plot_y_max, 
	int max_iteration, 
	unsigned int colour_0,
	unsigned int colour_1,
	unsigned int colour_2,
	unsigned int colour_3
) {
	
	Py_intptr_t shape[3] = { img_height, img_width, 3 };
	np::ndarray img = np::empty(3, shape, np::dtype::get_builtin<uint8_t>());
	char *data = img.get_data();

	double width_fraction = 1.0 / img_width * plot_width;
	double height_fraction = 1.0 / img_height * plot_height;

	int colour_0_r = (unsigned char)(colour_0);
	int colour_0_g = (unsigned char)(colour_0 >> 8);
	int colour_0_b = (unsigned char)(colour_0 >> 16);
	int colour_1_r = (unsigned char)(colour_1);
	int colour_1_g = (unsigned char)(colour_1 >> 8);
	int colour_1_b = (unsigned char)(colour_1 >> 16);
	int colour_2_r = (unsigned char)(colour_2);
	int colour_2_g = (unsigned char)(colour_2 >> 8);
	int colour_2_b = (unsigned char)(colour_2 >> 16);
	int colour_3_r = (unsigned char)(colour_3);
	int colour_3_g = (unsigned char)(colour_3 >> 8);
	int colour_3_b = (unsigned char)(colour_3 >> 16);

	cp::device device = cp::system::default_device();
	cp::context context = cp::context(device);
	cp::command_queue queue(context, device);
	int num_of_pixels = img_width * img_height;
	cp::vector<uint8_t> device_vector(num_of_pixels * 3, context);
	
	cp::copy(
		data, data + device_vector.size(), device_vector.begin(), queue
	);

	std::string source =
		"__kernel void mandelbrot("
		"	__global uchar *data,"
		"	const int img_width,"
		"	const int img_height,"
		"	const double width_fraction,"
		"	const double height_fraction,"
		"	const double plot_x_min,"
		"	const double plot_y_max,"
		"	const int max_iteration,"
		"	const int colour_0_r,"
		"	const int colour_0_g,"
		"	const int colour_0_b,"
		"	const int colour_1_r,"
		"	const int colour_1_g,"
		"	const int colour_1_b,"
		"	const int colour_2_r,"
		"	const int colour_2_g,"
		"	const int colour_2_b,"
		"	const int colour_3_r,"
		"	const int colour_3_g,"
		"	const int colour_3_b"
		") {"
		"	const uint id = get_global_id(0);"
		"	const uint i = id * 3;"
		"	int img_y = id / img_width;"
		"	int img_x = id % img_width;"
		""
		"	double plot_x = img_x * width_fraction + plot_x_min;"
		"	double plot_y = plot_y_max - img_y * height_fraction;"
		""
		"	double x = 0.0;"
		"	double y = 0.0;"
		"	int iteration = 0;"
		""
		"	double x_sq = 0;"
		"	double y_sq = 0;"
		""
		"	while (x_sq + y_sq < 4 && iteration < max_iteration) {"
		""
		"		y = 2 * x*y + plot_y;"
		"		x = x_sq - y_sq + plot_x;"
		""
		"		x_sq = x * x;"
		"		y_sq = y * y;"
		""
		"		iteration++;"
		"	}"
		"	if (iteration < max_iteration) {"
		"		int bits_per_colour = 8;"
		"		int intensity = iteration % (1 << bits_per_colour);"
		"		if (iteration < (1 << bits_per_colour)) {"
		"			data[i]     = colour_0_r + (((colour_1_r - colour_0_r) * intensity) >> bits_per_colour);"
		"			data[i + 1] = colour_0_g + (((colour_1_g - colour_0_g) * intensity) >> bits_per_colour);"
		"			data[i + 2] = colour_0_b + (((colour_1_b - colour_0_b) * intensity) >> bits_per_colour);"
		"		} else if (iteration < (2 << bits_per_colour)){"
		"			data[i]     = colour_1_r + (((colour_2_r - colour_1_r) * intensity) >> bits_per_colour);"
		"			data[i + 1] = colour_1_g + (((colour_2_g - colour_1_g) * intensity) >> bits_per_colour);"
		"			data[i + 2] = colour_1_b + (((colour_2_b - colour_1_b) * intensity) >> bits_per_colour);"
		"		} else {"
		"			data[i]     = colour_2_r;"
		"			data[i + 1] = colour_2_g;"
		"			data[i + 2] = colour_2_b;"
		"		}"
		"	}"
		"	else {"
		"		data[i]     = colour_3_r;"
		"		data[i + 1] = colour_3_g;"
		"		data[i + 2] = colour_3_b;"
		"	}"
		"}";
	boost::shared_ptr<cp::program_cache> global_cache = cp::program_cache::get_global_cache(context);
	std::string options;
	std::string key = "__pynq_mandelbrot";
	cp::program program = global_cache->get_or_build(key, options, source, context);
	cp::kernel kernel = program.create_kernel("mandelbrot");
	kernel.set_args(
		device_vector, 
		img_width, 
		img_height, 
		width_fraction, 
		height_fraction, 
		plot_x_min, 
		plot_y_max, 
		max_iteration, 
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
		colour_3_b
	);

	queue.enqueue_1d_range_kernel(kernel, 0, num_of_pixels, 0);

	cp::copy(
		device_vector.begin(), device_vector.end(), data, queue
	);

	return img;
}



BOOST_PYTHON_MODULE(Mandelbrot) {
	np::initialize();
	bp::def("calculate", calculate);
}