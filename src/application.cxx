#include <iostream>
#include <vector>
#include <cstdint>

#include <lodepng/lodepng.h>

#include <application.hxx>
#include <worker.hxx>

namespace fractal
{
	application::application(const configuration& p_cfg)
		: m_Cfg{p_cfg}, m_Context{p_cfg}, m_Renderer{p_cfg}
	{
	
	}

	auto application::run()
		-> void
	{			
		generate_image();
		
		init_threadpool();
	
		if(!m_Cfg.m_NoGraphics)
			game_loop();
		else
			m_IoService.run();
		
		stop_threadpool();
		
		if(!m_Cfg.m_ImagePath.empty())
			write_image();
	}
	
	auto application::game_loop()
		-> void
	{
		while(!m_Context.should_close())
		{		
			m_Context.pump_events();
			
			m_Context.begin_frame();
			
			m_Renderer.render();
			
			m_Context.end_frame();
		}
	}
	
	auto application::write_image()
		-> void
	{
		const auto& t_img = m_Renderer.current_image();
		
		::std::lock_guard<::std::mutex> t_lck{ t_img.buffer_mutex() };
	
		::std::vector<::std::uint8_t> t_buf{ };
		t_buf.resize(t_img.pixel_count() * 4U);
		
		// TODO flip?
		for(::std::size_t t_ix = 0; t_ix < t_img.pixel_count(); ++t_ix)
		{
			const auto t_base = t_ix * 4U;
			const auto& t_pixel = t_img.buffer()[t_ix];
			
			t_buf[t_base] = glm::clamp<::std::uint8_t>(t_pixel.r * 255U, 0U, 255U);
			t_buf[t_base + 1] = glm::clamp<::std::uint8_t>(t_pixel.g * 255U, 0U, 255U);
			t_buf[t_base + 2] = glm::clamp<::std::uint8_t>(t_pixel.b * 255U, 0U, 255U);
			t_buf[t_base + 3] = glm::clamp<::std::uint8_t>(t_pixel.a * 255U, 0U, 255U);
		}
		
		::std::vector<::std::uint8_t> t_outBuf{ };
		
		lodepng::encode(t_outBuf, t_buf, m_Cfg.m_WindowSize.x, m_Cfg.m_WindowSize.y);
		lodepng::save_file(t_outBuf, m_Cfg.m_ImagePath);
	}
	
	auto application::generate_image()
		-> void
	{
		// TODO Redo this! This is VERY ugly
	
		::std::vector<image_region> t_regions{ };
		
		//bool t_vertical{true};
		
		if(m_Cfg.m_Divisions == 0U)
			t_regions.push_back({ {0,0}, m_Cfg.m_WindowSize-dimension_type{1,1} });
		else
		{	
			// Region height
			const unsigned t_height = m_Cfg.m_WindowSize.y / m_Cfg.m_Divisions;
			
			// Piece that might be left over
			const unsigned t_leftOver = m_Cfg.m_WindowSize.y % t_height;
			
			for(::std::size_t t_i = 0; t_i < m_Cfg.m_Divisions; ++t_i)
			{	
				t_regions.push_back({
					{ 0, t_i * t_height},								// BL
					{ m_Cfg.m_WindowSize.x - 1U, (t_i+1) * t_height}	// TR
				});
			
				/*::std::vector<image_region> t_nextRegions{ };
				
				for(const auto& t_region: t_regions)
				{
					if(t_vertical)
					{
						t_nextRegions.push_back({
							t_region.bottom_left(),
							{ t_region.top_right().x/2, t_region.top_right().y }
						});
						
						t_nextRegions.push_back({
							{ t_region.bottom_left().x + (t_region.size().x/2), t_region.top_right().y },
							t_region.top_right()						
						});
					}
					else
					{
						t_nextRegions.push_back({
							t_region.bottom_left(),
							{ t_region.top_right().x, t_region.top_right().y/2 }
						});
						
						t_nextRegions.push_back({
							{ t_region.bottom_left().x, t_region.top_right().y + (t_region.size().y/2) },
							t_region.top_right()
						});
					}
				}
			
				// Replace old regions with new ones
				t_regions.swap(t_nextRegions);
				
				// Switch directions
				t_vertical = !t_vertical;*/
			}
			
			// Add remaining partition
			if(t_leftOver > 0U)
			{
				t_regions.push_back({
					{ 0, m_Cfg.m_Divisions * t_height},											// BL
					{ m_Cfg.m_WindowSize.x - 1U, (m_Cfg.m_Divisions * t_height) + t_leftOver}	// TR
				});
			}
		}
		
		// Post work orders
		for(const auto& t_region: t_regions)
		{
			m_IoService.post(worker{
				m_Cfg, m_Renderer.current_image(), t_region
			});
		}
	}
	
	auto application::init_threadpool()
		-> void
	{
		if(!m_Cfg.m_NoGraphics)
			m_Worker = ::std::make_unique<worker_type>(m_IoService);
		
		for(auto t_ix = 0; t_ix < m_Cfg.m_ThreadCount; ++t_ix)
			m_Pool.create_thread(boost::bind(&boost::asio::io_service::run, &m_IoService));		
	}
	
	auto application::stop_threadpool()
		-> void
	{
		m_IoService.stop();
		m_Pool.join_all();
	}
}
