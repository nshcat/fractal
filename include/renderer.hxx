#pragma once

#include <GLXW/glxw.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <types.hxx>
#include <image.hxx>
#include <configuration.hxx>
#include <empty_vbo.hxx>
#include <frame_counter.hxx>
#include <gl/program.hxx>

namespace fractal
{
	class renderer
	{
		public:
			renderer(const configuration&);
			
		public:
			auto render()
				-> void;
				
		protected:
			// Create texture and fill black
			auto create_texture()
				-> void;
			
			auto load_shaders()
				-> void;
				
			auto sync()
				-> void;
				
			auto set_uniforms()
				-> void;
				
			auto use_texture()
				-> void;			
	
		protected:
			GLuint m_GPUTex{ };
	
		protected:
			image m_Image;
			configuration m_Config{ };
			empty_vbo m_Vbo{ };
			frame_counter m_Counter{ 10U };
			gl::program m_Program{gl::defer_creation};
	};
}
