#ifndef __ZOMBYE_SKELETON_MANAGER_HPP__
#define __ZOMBYE_SKELETON_MANAGER_HPP__

#include <memory>
#include <string>

#include <zombye/rendering/skeleton.hpp>
#include <zombye/utils/cached_resource_manager.hpp>

namespace zombye {
    class game;
}

namespace zombye {
    using skeleton_ptr = std::shared_ptr<const skeleton>;

    class skeleton_manager : public cached_resource_manager<const skeleton, skeleton_manager> {
        friend class cached_resource_manager<const skeleton, skeleton_manager>;

        game& game_;
    public:
        skeleton_manager(game& game) noexcept;
        ~skeleton_manager() noexcept = default;
    protected:
        skeleton_ptr load_new(const std::string& name);
    };
}

#endif
