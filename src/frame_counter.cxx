#include <frame_counter.hxx>

namespace fractal
{
	frame_counter::frame_counter(::std::size_t p_limit)
		: m_FrameLimit{p_limit}
	{
		if(!p_limit)
			throw ::std::runtime_error("frame_counter: frame limit can't be 0!");
	}
	
	frame_counter::operator bool()
	{
		return should_fire();
	}
	
	auto frame_counter::should_fire()
		-> bool
	{
		++m_FrameCount;
		
		if(m_FrameCount >= m_FrameLimit)
		{
			m_FrameCount = 0U;
			return true;
		}
		else return false;
	}
}
