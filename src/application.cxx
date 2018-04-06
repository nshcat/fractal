#include <iostream>

#include <application.hxx>

namespace fractal
{
	application::application(const configuration& p_cfg)
		: m_Cfg{p_cfg}, m_Context{p_cfg.m_WindowSize}
	{
		
	}

	auto application::run()
		-> void
	{
		init_threadpool();
	
		while(!m_Context.should_close())
		{		
			m_Context.pump_events();
			
			m_Context.begin_frame();
			
			m_Context.end_frame();
		}
		
		stop_threadpool();
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
