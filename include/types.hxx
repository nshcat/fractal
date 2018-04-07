#pragma once

#include <glm/glm.hpp>

namespace fractal
{
	// Type used to represent a RGBA color tuple.
	using color_type = glm::vec4;
	
	// Type used to represent an integral, two dimensional size
	using dimension_type = glm::uvec2;
	
	// Type used to represent an integral, two dimensional point
	using position_type = glm::uvec2;
}
