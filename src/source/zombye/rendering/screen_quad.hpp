#ifndef __ZOMBYE_SCREEN_QUAD_HPP__
#define __ZOMBYE_SCREEN_QUAD_HPP__

#include <zombye/rendering/vertex_array.hpp>
#include <zombye/rendering/buffer.hpp>

namespace zombye {
	class rendering_system;
}

namespace zombye {
	class screen_quad {
	private:
		vertex_array vao_;
		vertex_buffer vbo_;

	public:
		screen_quad(rendering_system& renderer, const glm::vec2& top_left, const glm::vec2& bottom_right);
		~screen_quad() = default;

		screen_quad(const screen_quad& rhs) = delete;
		screen_quad& operator=(const screen_quad& rhs) = delete;

		screen_quad(screen_quad&& rhs) = delete;
		screen_quad& operator=(screen_quad&& rhs) = delete;

		void draw() const;
	};
}

#endif
