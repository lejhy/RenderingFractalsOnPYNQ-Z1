#define BOOST_PYTHON_STATIC_LIB
#define BOOST_NUMPY_STATIC_LIB
#define BOOST_COMPUTE_STATIC_LIB

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/compute.hpp>

typedef unsigned char	uchar;
typedef unsigned int	uint;

namespace bp = boost::python;
namespace np = boost::python::numpy;
namespace cp = boost::compute;

np::ndarray calculate(
	int img_width, 
	int img_height, 
	double width_fraction,
	double height_fraction,
	double plot_x_min, 
	double plot_y_max, 
	uint max_iteration,
	uint colour_span,
	uint colour_0,
	uint colour_1,
	uint colour_2,
	uint colour_3
) {
	
	Py_intptr_t shape[3] = { img_height, img_width, 3 };
	np::ndarray img = np::empty(3, shape, np::dtype::get_builtin<uint8_t>());
	char *data = img.get_data();

	uchar colour_0_r = colour_0 >> 16;
	uchar colour_0_g = colour_0 >> 8;
	uchar colour_0_b = colour_0;
	uchar colour_1_r = colour_1 >> 16;
	uchar colour_1_g = colour_1 >> 8;
	uchar colour_1_b = colour_1;
	uchar colour_2_r = colour_2 >> 16;
	uchar colour_2_g = colour_2 >> 8;
	uchar colour_2_b = colour_2;
	uchar colour_3_r = colour_3 >> 16;
	uchar colour_3_g = colour_3 >> 8;
	uchar colour_3_b = colour_3;

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
		"	const uchar colour_span,"
		"	const uchar colour_0_r,"
		"	const uchar colour_0_g,"
		"	const uchar colour_0_b,"
		"	const uchar colour_1_r,"
		"	const uchar colour_1_g,"
		"	const uchar colour_1_b,"
		"	const uchar colour_2_r,"
		"	const uchar colour_2_g,"
		"	const uchar colour_2_b,"
		"	const uchar colour_3_r,"
		"	const uchar colour_3_g,"
		"	const uchar colour_3_b"
		") {"
		"	const uint id = get_global_id(0);"
		"	const uint i = id * 3;"
		"	uint img_y = id / img_width;"
		"	uint img_x = id % img_width;"
		""
		"	double plot_x = img_x * width_fraction + plot_x_min;"
		"	double plot_y = plot_y_max - img_y * height_fraction;"
		""
		"	double x = 0.0;"
		"	double y = 0.0;"
		"	uint iteration = 0;"
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
		"	uchar intensity = 0;"
		"	uchar colour_a_r = 0;"
		"	uchar colour_a_g = 0;"
		"	uchar colour_a_b = 0;"
		"	uchar colour_b_r = 0;"
		"	uchar colour_b_g = 0;"
		"	uchar colour_b_b = 0;"
		"	if (iteration < max_iteration) {"
		"		if (colour_span < 8) {"
		"			iteration = iteration << (8 - colour_span);"
		"			intensity = iteration;"
		"		} else {"
		"			iteration = iteration >> (colour_span - 8);"
		"			intensity = iteration;"
		"		}"
		"		if (iteration < 256) {"
		"			colour_a_r = colour_0_r;"
		"			colour_a_g = colour_0_g;"
		"			colour_a_b = colour_0_b;"
		"			colour_b_r = colour_1_r;"
		"			colour_b_g = colour_1_g;"
		"			colour_b_b = colour_1_b;"
		"		} else if (iteration < 512){"
		"			colour_a_r = colour_1_r;"
		"			colour_a_g = colour_1_g;"
		"			colour_a_b = colour_1_b;"
		"			colour_b_r = colour_2_r;"
		"			colour_b_g = colour_2_g;"
		"			colour_b_b = colour_2_b;"
		"		} else {"
		"			colour_a_r = colour_2_r;"
		"			colour_a_g = colour_2_g;"
		"			colour_a_b = colour_2_b;"
		"			colour_b_r = colour_2_r;"
		"			colour_b_g = colour_2_g;"
		"			colour_b_b = colour_2_b;"
		"		}"
		"	}"
		"	else {"
		"			colour_a_r = colour_3_r;"
		"			colour_a_g = colour_3_g;"
		"			colour_a_b = colour_3_b;"
		"			colour_b_r = colour_3_r;"
		"			colour_b_g = colour_3_g;"
		"			colour_b_b = colour_3_b;"
		"	}"
		"	data[i]     = colour_a_r + (((colour_b_r - colour_a_r) * intensity) >> 8);"
		"	data[i + 1] = colour_a_g + (((colour_b_g - colour_a_g) * intensity) >> 8);"
		"	data[i + 2] = colour_a_b + (((colour_b_b - colour_a_b) * intensity) >> 8);"
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
		(uchar)colour_span,
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