#include <stdexcept>

#include <image_region.hxx>

namespace fractal
{
	image_region::image_region(const position_type& p_bl, const position_type& p_tr)
		: m_BL{p_bl}, m_TR{p_tr}
	{
		sanity_check();
	}
	
	auto image_region::sanity_check() const
		-> void
	{
		/*if((m_BL.x > m_TR.x) || (m_BL.y > m_TR.y))
			throw ::std::runtime_error("image_region: sanity check failed because m_TL > m_BR");*/
	}
	
	// Width and height of the region
	auto image_region::size() const
		-> dimension_type
	{
		return { m_TR.x - m_BL.x, m_TR.y - m_BL.y };
	}
		
	auto image_region::top_right() const
		-> const position_type&
	{
		return m_TR;
	}
		
	auto image_region::bottom_left() const
		-> const position_type&
	{
		return m_BL;
	}
	
	auto image_region::calculate_offset(const position_type& p_pos) const
		-> ::std::size_t
	{
		return (p_pos.y * size().x) + p_pos.x;
	}
		
	// Number of pixels in this region
	auto image_region::pixel_count() const
		-> ::std::size_t
	{
		const auto t_sz = size();
		return t_sz.x * t_sz.y;
	}
}
