#ifndef __ZOMBYE_TEXTURE_MANAGER_HPP__
#define __ZOMBYE_TEXTURE_MANAGER_HPP__

#include <memory>

#include <zombye/rendering/texture.hpp>
#include <zombye/utils/cached_resource_manager.hpp>

namespace zombye {
    class game;
}

namespace zombye {
    using texture_ptr = std::shared_ptr<const texture>;

    class texture_manager : public cached_resource_manager<const texture, texture_manager> {
        friend class cached_resource_manager<const texture, texture_manager>;

        game& game_;
    public:
        texture_manager(game& game) noexcept;
        ~texture_manager() noexcept = default;
    protected:
        texture_ptr load_new(const std::string& name);
    };
}

#endif
