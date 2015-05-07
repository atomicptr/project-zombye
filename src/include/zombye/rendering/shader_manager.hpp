#ifndef __ZOMBYE_SHADER_MANAGER_HPP__
#define __ZOMBYE_SHADER_MANAGER_HPP__

#include <memory>
#include <string>

#include <zombye/utils/cached_resource_manager.hpp>

namespace zombye {
    class game;
    class shader;
}

namespace zombye {
    using shader_ptr = std::shared_ptr<const shader>;

    class shader_manager : public cached_resource_manager<const shader, shader_manager> {
        friend class cached_resource_manager<const shader, shader_manager>;

        game& game_;
    public:
        shader_manager(game& game) noexcept;
        ~shader_manager() noexcept = default;
    protected:
        shader_ptr load_new(const std::string& name, GLenum type);
    };
}

#endif
