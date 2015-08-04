#ifndef __ZOMBYE_MESH_MANAGER_HPP__
#define __ZOMBYE_MESH_MANAGER_HPP__

#include <memory>

#include <zombye/utils/cached_resource_manager.hpp>

namespace zombye {
    class game;
    class mesh;
    class rendering_system;
}

namespace zombye {
    using mesh_ptr = std::shared_ptr<const mesh>;

    class mesh_manager : public cached_resource_manager<const mesh, mesh_manager> {
        friend class cached_resource_manager<const mesh, mesh_manager>;

        game& game_;
        rendering_system& rendering_system_;
    public:
        mesh_manager(game& game, rendering_system& rendering_system) noexcept;
        ~mesh_manager() noexcept = default;
    protected:
        mesh_ptr load_new(const std::string& name);
    };
}

#endif
