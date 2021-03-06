#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

#include <render_context.hxx>
#include <configuration.hxx>
#include <renderer.hxx>


namespace fractal
{
	// Handles main render loop and context creation
	class application
	{
		using worker_type = boost::asio::io_service::work;
		using worker_ptr = ::std::unique_ptr<worker_type>;
	
		public:
			application(const configuration&);
			
		public:
			auto run()
				-> void;
			
		protected:
			auto init_threadpool()
				-> void;
				
			auto stop_threadpool()
				-> void;
				
			auto generate_image()
				-> void;
			
			auto write_image()
				-> void;
			
			auto game_loop()
				-> void;
	
		protected:
			configuration m_Cfg{ };
			render_context m_Context;
			renderer m_Renderer;
			boost::asio::io_service m_IoService{ };
			worker_ptr m_Worker{ };
			boost::thread_group m_Pool{ };		
	};
}
