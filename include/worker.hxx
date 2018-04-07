#pragma once

#include <image.hxx>
#include <configuration.hxx>

namespace fractal
{
	// Functor object used to compute a region of the fractal image
	// TODO: Rename "job"
	class worker
	{
		// TODO later on add complex_mapping
		public:
			worker(const configuration&, image&, const image_region&);
			
			auto operator()()
				-> void;	
				
		protected:
			auto map_point(const position_type&)
				-> ::std::complex<double>;				
	
		protected:
			image& m_Image;
			configuration m_Config{ };
			image_region m_Region;
	};
}
