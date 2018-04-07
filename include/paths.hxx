#pragma once

#include <boost/filesystem.hpp>

namespace fractal
{
	class paths
	{
		using path_type = boost::filesystem::path;
	
		static auto working_directory()
			-> path_type;
	};
}
