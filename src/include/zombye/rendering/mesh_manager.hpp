#ifndef __ZOMBYE_MESH_MANAGER_HPP__
#define __ZOMBYE_MESH_MANAGER_HPP__

#include <memory>
#include <string>

#include <zombye/rendering/mesh.hpp>
#include <zombye/utils/cached_resource_manager.hpp>

namespace zombye {
    using mesh_ptr = std::shared_ptr<const mesh>;

    class rendering_system;
    class mesh_manager : public cached_resource_manager<const mesh, mesh_manager> {
        friend class cached_resource_manager<const mesh, mesh_manager>;
        rendering_system& rendering_system_;
    public:
        mesh_manager(rendering_system& rendering_system) noexcept;
        ~mesh_manager() noexcept = default;
    protected:
        mesh_ptr load_new(const std::string& name);
    };
}

#endif