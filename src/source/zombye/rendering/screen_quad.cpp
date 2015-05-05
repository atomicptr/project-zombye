#include <glm/glm.hpp>

#include <zombye/rendering/rendering_system.hpp>
#include "screen_quad.hpp"
#include <zombye/rendering/mesh.hpp>

namespace zombye {
	screen_quad::screen_quad(rendering_system& renderer, const glm::vec2& top_left, const glm::vec2& bottom_right)
	: vbo_{6 * sizeof(vertex), GL_STATIC_DRAW} {
		glm::vec3 zero{0.f};
		vertex vertices[6] = {
			{glm::vec3{top_left, 0.f},						glm::vec2{0.f, 1.f}, zero},// zero, zero},
			{glm::vec3{top_left.x, bottom_right.y, 0.f},	glm::vec2{0.f, 0.f}, zero},// zero, zero},
			{glm::vec3{bottom_right.x, top_left.y, 0.f}, 	glm::vec2{1.f, 1.f}, zero},// zero, zero},
			{glm::vec3{top_left.x, bottom_right.y, 0.f}, 	glm::vec2{0.f, 0.f}, zero},// zero, zero},
			{glm::vec3{bottom_right, 0.f},					glm::vec2{1.f, 0.f}, zero},// zero, zero},
			{glm::vec3{bottom_right.x, top_left.y, 0.f}, 	glm::vec2{1.f, 1.f}, zero},// zero, zero}
		};
		vbo_.data(6 * sizeof(vertex), vertices);
		renderer.staticmesh_layout().setup_layout(vao_, &vbo_);
	}

	void screen_quad::draw() const {
		vao_.bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}
