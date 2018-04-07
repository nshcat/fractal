#include <iostream>
#include <vector>

#include <application.hxx>
#include <worker.hxx>

namespace fractal
{
	application::application(const configuration& p_cfg)
		: m_Cfg{p_cfg}, m_Context{p_cfg.m_WindowSize}, m_Renderer{p_cfg}
	{
		// TODO MAYBE BUG: If context here is initialized AFTER renderer, nothing will work!
	}

	auto application::run()
		-> void
	{
		init_threadpool();
				
		generate_image();
	
		while(!m_Context.should_close())
		{		
			m_Context.pump_events();
			
			m_Context.begin_frame();
			
			m_Renderer.render();
			
			m_Context.end_frame();
		}
		
		stop_threadpool();
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
