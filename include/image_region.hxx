#pragma once

#include <types.hxx>

namespace fractal
{
	// A class representing a rectangular region of the complete image.
	// It is defined by two points: the bottom left and top right point.
	// Remember that in OpenGL, textures (and therefore this image) have their
	// origin (0,0) at the bottom left corner.
	class image_region
	{
		public:
			image_region(const position_type& p_bl, const position_type& p_tr);
		
		public:
			// Width and height of the region
			auto size() const
				-> dimension_type;
				
			auto top_right() const
				-> const position_type&;
				
			auto bottom_left() const
				-> const position_type&;
				
			// Number of pixels in this region
			auto pixel_count() const
				-> ::std::size_t;
				
			// Given a relative position in relation to bottom left,
			// calculate the offset of that pixel in a buffer
			// representing this region.
			auto calculate_offset(const position_type&) const
				-> ::std::size_t;
		
		protected:
			// Check that TL <= BR
			auto sanity_check() const
				-> void;
	
		protected:
			//dimension_type m_ImageSize;	//< Size of the complete image
			position_type m_TR;
			position_type m_BL;
	};
}
