#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <ut/integral.hxx>

#include <renderer.hxx>
#include <paths.hxx>

#include <gl/uniform.hxx>

using namespace ut::literals;

namespace fractal
{
	renderer::renderer(const configuration& p_cfg)
		: m_Image{p_cfg.m_WindowSize}, m_Config{p_cfg}
	{
		if(!p_cfg.m_NoGraphics)
		{
			create_texture();
			load_shaders();
			set_uniforms();
		}
	}
	
	auto renderer::render()
		-> void
	{
		if(m_Counter)
			sync();
			
		use_texture();
		m_Vbo.use();
		m_Program.use();
		set_uniforms();
		m_Vbo.initialize();
		
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);
	}
	
	auto renderer::use_texture()
		-> void
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_GPUTex);		
	}
	
	auto renderer::current_image()
		-> image&
	{
		return m_Image;
	}
	
	auto renderer::create_texture()
		-> void
	{
		const auto t_winSz = m_Config.m_WindowSize;
		
		LOG_D_TAG("renderer") << t_winSz.x << ", " << t_winSz.y;
	
		// Create texture object
		glGenTextures(1, &m_GPUTex);
		
		// Texture is a default 2D texture
		glBindTexture(GL_TEXTURE_2D, m_GPUTex);
		
		// All black (TODO: For now white for debugging purposes)
		::std::vector<color_type> t_Buf(t_winSz.x * t_winSz.y, color_type{ 0.f, 0.f, 0.f, 1.f });	
		
		//t_Buf[0] = color_type{ 0.f, 1.f, 0.f, 1.f };
		
		/*for(unsigned t_dx = 0; t_dx < t_winSz.x; ++t_dx)
		{
			for(unsigned t_dy = 0; t_dy < t_winSz.y; ++t_dy)
			{
					t_Buf[m_Image.calculate_offset({t_dx, t_dy})] =
						((t_dy % 2) == 0) ? color_type{ 1.f, 1.f, 1.f, 1.f } : color_type{ 0.f, 0.f, 0.f, 1.f };
			}	
		}*/
		
		
		/*glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, t_winSz.x, t_winSz.y);*/
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, t_winSz.x, t_winSz.y,
			0, GL_RGBA, GL_FLOAT, reinterpret_cast<const GLvoid*>(t_Buf.data())
		);
		
		//glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, t_winSz.x, t_winSz.y);
		
		/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, t_winSz.x, t_winSz.y,
			0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<const GLvoid*>(t_Buf.data())
		);*/
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	}
	
	auto renderer::set_uniforms()
		-> void
	{
		// Projection matrix
		const auto t_proj =  glm::ortho(0.f, static_cast<float>(m_Config.m_WindowSize.x), static_cast<float>(m_Config.m_WindowSize.y), 0.f, -1.f, 1.f);
		gl::set_uniform(m_Program, "projection_mat", t_proj);
		
		// Window size
		gl::set_uniform(m_Program, "window_size", m_Config.m_WindowSize);
	}
	
	auto renderer::sync()
		-> void
	{
		// Lock image buffer mutex
		::std::lock_guard<::std::mutex> t_lck(m_Image.buffer_mutex());
		
		if(m_Image.is_dirty())
		{
			glBindTexture(GL_TEXTURE_2D, m_GPUTex);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Config.m_WindowSize.x, m_Config.m_WindowSize.y,
				0, GL_RGBA, GL_FLOAT, reinterpret_cast<const GLvoid*>(&m_Image.buffer()[0])
			);
		
			/*for(const auto& t_region: m_Image.dirty_regions())
			{
				// Calculate pointer
				auto t_data = reinterpret_cast<const GLvoid*>(
					&(m_Image.buffer()[m_Image.calculate_offset(t_region.bottom_left())])
				);
			
				// Update region in texture
				glTexSubImage2D(GL_TEXTURE_2D, 0, t_region.bottom_left().x, t_region.bottom_left().y,
					 t_region.size().x, t_region.size().y, GL_RGBA, GL_FLOAT, t_data
				);						
			}
			*/
			
			m_Image.dirty_regions().clear();
			m_Image.set_dirty(false);
		}
	}
	
	auto renderer::load_shaders()
		-> void
	{
		try
		{
			m_Program = gl::program{
				gl::vertex_shader{ gl::from_file, "assets/shaders/main.vs.glsl" },
				gl::fragment_shader{ gl::from_file, "assets/shaders/main.fs.glsl" }
			};
			
			m_Program.use();
		}
		catch(const gl::shader_exception& t_ex)
		{
			LOG_F_TAG("renderer") << "failed to compile shader:\n" << t_ex.log();
			std::exit(EXIT_FAILURE);
		}
	}
}
