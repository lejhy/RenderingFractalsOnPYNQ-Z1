#define BOOST_PYTHON_STATIC_LIB
#define BOOST_NUMPY_STATIC_LIB
#define BOOST_COMPUTE_STATIC_LIB

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/compute.hpp>

namespace bp = boost::python;
namespace np = boost::python::numpy;
namespace cp = boost::compute;

np::ndarray calculate(int img_width, int img_height, double plot_width, double plot_height, double plot_x_min, double plot_y_max, int max_iteration) {
	
	Py_intptr_t shape[3] = { img_height, img_width, 3 };
	np::ndarray img = np::empty(3, shape, np::dtype::get_builtin<uint8_t>());
	char *data = img.get_data();

	double width_fraction = 1.0 / img_width * plot_width;
	double height_fraction = 1.0 / img_height * plot_height;

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
		"	const int max_iteration"
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
		"		int colour = iteration % 512;"
		"		if (colour > 255) {"
		"			colour = colour - 256;"
		"			data[i] = colour;"
		"			data[i + 1] = 0xFF;"
		"			data[i + 2] = colour;"
		"		}"
		"		else {"
		"			data[i] = 0x00;"
		"			data[i + 1] = colour;"
		"			data[i + 2] = 0x00;"
		"		}"
		"	}"
		"	else {"
		"		data[i] = 0x00;"
		"		data[i + 1] = 0x00;"
		"		data[i + 2] = 0x00;"
		"	}"
		"}";
	boost::shared_ptr<cp::program_cache> global_cache = cp::program_cache::get_global_cache(context);
	std::string options;
	std::string key = "__pynq_mandelbrot";
	cp::program program = global_cache->get_or_build(key, options, source, context);
	cp::kernel kernel = program.create_kernel("mandelbrot");
	kernel.set_args(device_vector, img_width, img_height, width_fraction, height_fraction, plot_x_min, plot_y_max, max_iteration);

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