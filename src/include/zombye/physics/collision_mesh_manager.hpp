#ifndef __ZOMBYE_COLLISION_MESH_MANAGER_HPP__
#define __ZOMBYE_COLLISION_MESH_MANAGER_HPP__

#include <memory>

#include <zombye/physics/collision_mesh.hpp>
#include <zombye/utils/cached_resource_manager.hpp>

namespace zombye {
    class game;
}

namespace zombye {
    using collision_mesh_ptr = std::shared_ptr<const collision_mesh>;

    class collision_mesh_manager : public cached_resource_manager<const collision_mesh, collision_mesh_manager> {
        friend class cached_resource_manager<const collision_mesh, collision_mesh_manager>;

        game& game_;
    public:
        collision_mesh_manager(game& game) noexcept;
        ~collision_mesh_manager() noexcept = default;
    protected:
        collision_mesh_ptr load_new(const std::string& name);
    };
}

#endif