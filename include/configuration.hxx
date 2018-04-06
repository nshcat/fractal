#pragma once

#include <thread>

#include <types.hxx>

namespace fractal
{
	// Stores information that is configurable by the user
	struct configuration
	{
		dimension_type m_WindowSize{ 600, 400 };
		::std::size_t m_ThreadCount{ std::thread::hardware_concurrency() };
		::std::size_t m_Iterations{ 500 };
	};
}
