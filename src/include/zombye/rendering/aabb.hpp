#ifndef __ZOMBYE_AABB_HPP__
#define __ZOMBYE_AABB_HPP__

#include <glm/glm.hpp>

namespace zombye {
	struct aabb {
		glm::vec3 min;
		glm::vec3 max;
	};
}

#endif
