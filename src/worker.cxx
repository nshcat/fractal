#include <complex>
#include <vector>
#include <optional>
#include <cmath>
#include <log.hxx>

#include <worker.hxx>
#include <types.hxx>

namespace fractal
{
	worker::worker(const configuration& p_cfg, image& p_img, const image_region& p_reg)
		: m_Config{p_cfg}, m_Image{p_img}, m_Region{p_reg}
	{
	}
	
	auto worker::map_point(const position_type& p_pos)
		-> ::std::complex<double>
	{
		const glm::vec2 t_upperLeft{ -1.2f, 0.35f };
		const glm::vec2 t_lowerRight{ -1.f, 0.2f };
		
		const glm::vec2 t_size{
			t_lowerRight.x - t_upperLeft.x,
			t_upperLeft.y - t_lowerRight.y
		};
		
		const glm::vec2 t_result{
			t_upperLeft.x + (float)p_pos.x * t_size.x / (float)m_Config.m_WindowSize.x,
			t_upperLeft.y - (float)p_pos.y * t_size.y / (float)m_Config.m_WindowSize.y
		};
		
		return { t_result.x, t_result.y };
		
	}
	
	auto worker::operator()()
		-> void
	{
		// Create buffer
		::std::vector<color_type> t_buf{ };
		t_buf.resize(m_Region.pixel_count());
		
		// Iterate through all pixels in region
		const auto t_sz = m_Region.size();
		for(unsigned t_dx = 0; t_dx < t_sz.x; ++t_dx)
		{
			for(unsigned t_dy = 0; t_dy < t_sz.y; ++t_dy)
			{
				// Relative position
				const position_type t_pos{ t_dx, t_dy }; 
			
				// Calculate offset into the buffer
				const auto t_offset = m_Region.calculate_offset(t_pos);
				
				// Absolute position
				const auto t_abs = t_pos + m_Region.bottom_left();
				
				// Retrieve complex number
				const auto t_c = map_point(t_abs);
				
				// Perform mandelbrot test
				::std::complex<double> t_z{ 0.f, 0.f };
				
				::std::optional<::std::size_t> t_escapeTime{ };
				
				for(::std::size_t t_i = 0; t_i < m_Config.m_Iterations; ++t_i)
				{
					//t_z = (t_z * t_z) + t_c;
					
					t_z = (t_z * t_z) + t_c;
					
					const auto t_norm = ::std::norm(t_z) * ::std::norm(t_z);
					
					if(t_norm > 4.f)
					{
						t_escapeTime = t_i;
					}
				}
			
				const float t_val = t_escapeTime ? ((float)*t_escapeTime) / (float)m_Config.m_Iterations : 0.f;
				
				t_buf[t_offset] = color_type{ t_val, t_val, t_val, 1.f };
			}
		}
		
		{
			::std::lock_guard<::std::mutex> t_lck{ m_Image.buffer_mutex() };		
			m_Image.set_region(m_Region, { t_buf.begin(), t_buf.end() });
		}
	}
}
