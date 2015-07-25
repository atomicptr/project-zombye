#ifndef __ZOMBYE_BOUNDING_BOX_HPP__
#define __ZOMBYE_BOUNDING_BOX_HPP__

#include <array>

#include <glm/glm.hpp>

namespace zombye {
	struct bounding_box {
		std::array<glm::vec3, 8> points;
	};
}

#endif
