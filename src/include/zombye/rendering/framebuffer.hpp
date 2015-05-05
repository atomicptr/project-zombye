#ifndef __ZOMBYE_FRAMEBUFFER_HPP__
#define __ZOMBYE_FRAMEBUFFER_HPP__

#include <memory>
#include <unordered_map>

#include <GL/glew.h>

#include <zombye/rendering/texture.hpp>

namespace zombye {
	class framebuffer {
	private:
		GLuint id_;
		std::unordered_map<GLenum, std::unique_ptr<texture>> attachments_;

	public:
		framebuffer() noexcept;
		~framebuffer();

		framebuffer(const framebuffer& rhs) = delete;
		framebuffer& operator=(const framebuffer& rhs) = delete;

		framebuffer(framebuffer&& rhs) noexcept;
		framebuffer& operator=(framebuffer&& rhs) noexcept;

		void bind() const noexcept;

		template <typename... arguments>
		void attach(GLenum attachment, arguments&&... args) {
			bind();
			auto tex = std::make_unique<texture>(std::forward<arguments>(args)...);
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex->id_, 0);
			attachments_.insert(std::make_pair(attachment, std::move(tex)));
			bind_default();
		}

		texture& attachment(GLenum attachment) const;
		static void bind_default() noexcept;
	};
}

#endif
