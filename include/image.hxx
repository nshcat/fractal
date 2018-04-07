#pragma once

#include <vector>
#include <mutex>
#include <atomic>
#include <deque>

#include <ut/array_view.hxx>

#include <types.hxx>
#include <image_region.hxx>

namespace fractal
{
	// The origin is on the bottom left of the image.
	// This is the OpenGL convention.
	class image
	{
		using region_list = ::std::deque<image_region>;
		using buffer_type = ::std::vector<color_type>;
		using buffer_view = ut::array_view<color_type>;
		
		public:
			image(const dimension_type&);
			~image();

		// NOTE: All methods need to be called with buffer_mutex() locked
		public:
			auto size() const
				-> const dimension_type&;
				
			auto buffer_mutex() const
				-> ::std::mutex&;
				
			auto buffer() const
				-> buffer_view;
				
			auto is_dirty() const
				-> bool;
				
			auto calculate_offset(const position_type&) const
				-> ::std::size_t;
				
			auto dirty_regions()
				-> region_list&;
			
		public:
			auto set_region(const image_region&, buffer_view)
				-> void;

			auto set_dirty(bool)
				-> void;

			auto set_pixel(const position_type&, const color_type&)
				-> void;

			auto get_pixel(const position_type&) const
				-> const color_type&;
			
		protected:
			dimension_type m_Size{ };
			buffer_type m_Buffer{ };
			
		protected:
			::std::atomic_bool m_IsDirty{ true };
			region_list m_DirtyRegions{ };
			mutable ::std::mutex m_Mtx{ };	//< Mutex used to manage access to the image buffer
	};
}
