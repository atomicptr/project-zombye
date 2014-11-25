#ifndef __ZOMBYE_RIGGED_MESH_MANAGER_HPP__
#define __ZOMBYE_RIGGED_MESH_MANAGER_HPP__

#include <memory>
#include <string>

#include <zombye/rendering/rigged_mesh.hpp>
#include <zombye/utils/cached_resource_manager.hpp>

namespace zombye {
    using rigged_mesh_ptr = std::shared_ptr<const rigged_mesh>;

    class game;
    class rigged_mesh_manager : public cached_resource_manager<const rigged_mesh, rigged_mesh_manager> {
        friend class cached_resource_manager<const rigged_mesh, rigged_mesh_manager>;
        game& game_;
    public:
        rigged_mesh_manager(game& game) noexcept;
        ~rigged_mesh_manager() noexcept = default;
    protected:
        rigged_mesh_ptr load_new(const std::string& name);
    };
}

#endif