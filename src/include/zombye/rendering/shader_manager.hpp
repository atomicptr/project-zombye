#ifndef __ZOMBYE_SHADER_MANAGER_HPP__
#define __ZOMBYE_SHADER_MANAGER_HPP__

#include <memory>
#include <string>

#include <zombye/rendering/shader.hpp>
#include <zombye/utils/cached_resource_manager.hpp>

namespace zombye {
    using shader_ptr = std::shared_ptr<const shader>;

    class shader_manager : public cached_resource_manager<const shader, shader_manager> {
        friend class cached_resource_manager<const shader, shader_manager>;
    public:
        shader_manager() = default;
        ~shader_manager() noexcept = default;
    protected:
        shader_ptr load_new(const std::string& name, GLenum type);
    };
}

#endif