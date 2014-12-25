#ifndef __ZOMBYE_SKINNED_MESH_MANAGER_HPP__
#define __ZOMBYE_SKINNED_MESH_MANAGER_HPP__

#include <memory>
#include <string>

#include <zombye/rendering/skinned_mesh.hpp>
#include <zombye/utils/cached_resource_manager.hpp>

namespace zombye {
    class game;
}

namespace zombye {
    using skinned_mesh_ptr = std::shared_ptr<const skinned_mesh>;

    class skinned_mesh_manager : public cached_resource_manager<const skinned_mesh, skinned_mesh_manager> {
        friend class cached_resource_manager<const skinned_mesh, skinned_mesh_manager>;

        game& game_;
    public:
        skinned_mesh_manager(game& game) noexcept;
        ~skinned_mesh_manager() noexcept = default;
    protected:
        skinned_mesh_ptr load_new(const std::string& name);
    };
}

#endif