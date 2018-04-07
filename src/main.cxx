#include <cl.hxx>
#include <log.hxx>
#include <ut/integral.hxx>
#include <application.hxx>

using namespace ut::literals;

fractal::configuration g_config{ };
lg::severity_level g_logLevel{ };
bool g_verbose{ };

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
	},
	
	cl::enum_argument<lg::severity_level>{
		cl::long_name("verbosity"),
		cl::category("Logger"),
		cl::short_name('V'),
		cl::description("Sets the threshold for log messages to be shown"),
		cl::default_value(lg::severity_level::info),
		cl::ignore_case,
		cl::reference(g_logLevel),
		
		cl::enum_key_value("all", 		lg::severity_level::debug),
		cl::enum_key_value("debug", 	lg::severity_level::debug),
		cl::enum_key_value("info", 		lg::severity_level::info),
		cl::enum_key_value("warning", 	lg::severity_level::warning),
		cl::enum_key_value("error", 	lg::severity_level::error),
		cl::enum_key_value("fatal", 	lg::severity_level::fatal)
	},
	
	cl::boolean_argument
	{
		cl::long_name("verbose"),
		cl::category("Logger"),
		cl::short_name('v'),
		cl::default_value(false),
		cl::reference(g_verbose),
		cl::description("Enables verbose mode. Equivalent to \"-Vdebug\"")
	}
};


int main(int argc, const char** argv)
{
	// Parse command line
	g_handler.read(argc, argv);
	
	// Initialize logger
	lg::logger::null_init();
	const auto t_level = g_verbose ? lg::severity_level::debug : g_logLevel;	
	lg::console_target<lg::clang_formatter> t_target{ t_level };
	lg::logger::add_target(&t_target);
	
	// Run application
	fractal::application t_app{ g_config };
	t_app.run();
	
	// Shutdown logger
	lg::logger::shutdown();
}
