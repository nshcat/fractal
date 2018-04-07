#include <image.hxx>

namespace fractal
{
	image::image(const dimension_type& p_size)
		: m_Size{p_size}, m_Buffer(p_size.x * p_size.y, color_type{ })
	{
	}
	
	auto image::size() const
		-> const dimension_type&
	{
		return m_Size;
	}
		
	auto image::buffer_mutex() const
		-> ::std::mutex&
	{
		return m_Mtx;
	}
		
	auto image::buffer() const
		-> buffer_view
	{
		return { m_Buffer.begin(), m_Buffer.end() };
	}
		
	auto image::is_dirty() const
		-> bool
	{
		return m_IsDirty;
	}
	
	auto image::set_dirty(bool p_val)
		-> void
	{
		m_IsDirty = p_val;
	}
	
	auto image::set_pixel(const position_type& p_pos, const color_type& p_clr)
		-> void
	{
		m_Buffer[calculate_offset(p_pos)] = p_clr;
	}
				
	auto image::get_pixel(const position_type& p_pos) const
		-> const color_type&
	{
		return m_Buffer[calculate_offset(p_pos)];
	}
	
	auto image::dirty_regions()
		-> region_list&
	{
		return m_DirtyRegions;
	}
	
	auto image::calculate_offset(const position_type& p_pos) const
		-> ::std::size_t
	{
		// Quote from the OpenGL documentation:
		/*
			The first element corresponds to the lower left corner of the texture image.
			Subsequent elements progress left-to-right through the remaining texels in the
			lowest row of the texture image, and then in successively higher rows of the
			texture image. The final element corresponds to the upper right corner of the
			texture image.
		*/
		
		return (p_pos.y * size().x) + p_pos.x;		
	}
	
	auto image::set_region(const image_region& p_region, buffer_view p_data)
		-> void
	{			
		for(unsigned t_dx = 0; t_dx < p_region.size().x; ++t_dx)
		{
			for(unsigned t_dy = 0; t_dy < p_region.size().y; ++t_dy)
			{
				// Relative position in relation to bottom left
				const position_type t_pos{ t_dx, t_dy };
				
				const auto t_off = p_region.calculate_offset(t_pos);
				
				this->set_pixel(t_pos + p_region.bottom_left(), p_data[t_off]);
			}
		}
			
		m_DirtyRegions.push_back(p_region);
		
		set_dirty(true);
	}
}
