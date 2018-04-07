
#include <boost/filesystem/operations.hpp>

#include <paths.hxx>


namespace fractal
{
	auto paths::working_directory()
		-> path_type
	{
		return boost::filesystem::current_path();
	}
}
