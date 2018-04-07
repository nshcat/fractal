#pragma once

#include <stdexcept>

namespace fractal
{
	// Simple helper class allowing easy "do every n-th frame" kind of operations
	class frame_counter
	{
		public:
			frame_counter(::std::size_t p_limit);
			
		public:
			auto should_fire()
				-> bool;
				
			operator bool();
	
		protected:
			const ::std::size_t m_FrameLimit{ 1U };
			::std::size_t m_FrameCount{ 0U };
	};
}
