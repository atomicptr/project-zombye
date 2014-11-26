#ifndef __ZOMBYE_ANIMATION_MANAGER_HPP__
#define __ZOMBYE_ANIMATION_MANAGER_HPP__

#include <memory>
#include <string>

#include <zombye/rendering/animation.hpp>
#include <zombye/utils/cached_resource_manager.hpp>

namespace zombye {
    using animation_ptr = std::shared_ptr<const animation>;

    class game;
    class animation_manager : public cached_resource_manager<const animation, animation_manager> {
        friend class cached_resource_manager<const animation, animation_manager>;
        game& game_;
    public:
        animation_manager(game& game) noexcept;
        ~animation_manager() = default;
    protected:
        animation_ptr load_new(const std::string& name);
    };
}

#endif