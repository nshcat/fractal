#include <cl.hxx>
#include <ut/integral.hxx>
#include <application.hxx>

using namespace ut::literals;


fractal::configuration g_config{ };

cl::handler g_handler{
	cl::application_name("fractal"),
	
	cl::help_argument{ },
	
	cl::integer_argument<unsigned>{
		cl::long_name("width"),
		cl::short_name('W'),
		cl::description("Sets the width of the main window"),
		cl::category("Graphics"),
		cl::reference(g_config.m_WindowSize.x)
	},
	
	cl::integer_argument<unsigned>{
		cl::long_name("height"),
		cl::short_name('H'),
		cl::description("Sets the height of the main window"),
		cl::category("Graphics"),
		cl::reference(g_config.m_WindowSize.y)
	},
	
	cl::integer_argument<::std::size_t>{
		cl::long_name("thread-count"),
		cl::short_name('t'),
		cl::description("Sets the number of threads used by the rendering thread pool"),
		cl::reference(g_config.m_ThreadCount),
		cl::min(1_sz),
		cl::max(32_sz)
	},
	
	cl::integer_argument<::std::size_t>{
		cl::long_name("iterations"),
		cl::short_name('i'),
		cl::description("Sets the number of iterations spent analyzing each pixel"),
		cl::category("Graphics"),
		cl::reference(g_config.m_Iterations),
		cl::min(8_sz),
		cl::max(20000_sz)
	}
};


int main(int argc, const char** argv)
{
	g_handler.read(argc, argv);

	fractal::application t_app{ g_config };
	
	t_app.run();
}
