#include <zombye/core/game.hpp>
#include <zombye/ecs/component.hpp>
#include <zombye/rendering/staticmesh_component.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    staticmesh_component::staticmesh_component(game& game, entity& owner, const std::string& mesh)
    : reflective{game, owner} {
        game_.rendering_system().register_component(this);
        load(mesh);
    }

    staticmesh_component::~staticmesh_component() noexcept {
        game_.rendering_system().unregister_component(this);
    }

    void staticmesh_component::draw() const noexcept {
        mesh_->vao().bind();
        for (auto i = 0u; i < materials_.size(); ++i) {
            materials_[i].color->bind(0);
            materials_[i].specular->bind(1);
            mesh_->draw(i);
        }
    }

    void staticmesh_component::load(const std::string& mesh) {
        auto& rendering_system = game_.rendering_system();

        mesh_ = rendering_system.mesh_manager().load(mesh);
        if (!mesh_) {
            log(LOG_FATAL, "could not load mesh from file " + mesh);
        }

        auto asset = game_.asset_manager().load(mesh);
        if (!asset) {
            log(LOG_FATAL, "could not load asset " + mesh);
        }

        auto data_ptr = asset->content().data();
        auto size = *reinterpret_cast<const size_t*>(data_ptr) * sizeof(vertex);
        data_ptr += sizeof(size_t);
        data_ptr += size;
        size = *reinterpret_cast<const size_t*>(data_ptr) * sizeof(unsigned int);
        data_ptr += sizeof(size_t);
        data_ptr += size;
        size = *reinterpret_cast<const size_t*>(data_ptr) * sizeof(submesh);
        data_ptr += sizeof(size_t);
        data_ptr += size;

        auto material_count = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        auto& texture_manager = rendering_system.texture_manager();
        for (auto i = 0u; i < material_count; ++i) {
            size = *reinterpret_cast<const size_t*>(data_ptr);
            data_ptr += sizeof(size_t);
            auto name = std::string{data_ptr, size};
            data_ptr += size;

            material m;
            auto color = texture_manager.load("texture/" + name + "_color.dds");
            if (!color) {
                log(LOG_FATAL, "could not load texture " + name + "_color.dds");
            }
            m.color = color;

            auto specular = texture_manager.load("texture/" + name + "_specular.dds");
            if (!color) {
                log(LOG_FATAL, "could not load texture " + name + "_specular.dds");
            }
            m.specular = specular;

            materials_.emplace_back(m);
        }
    }

    staticmesh_component::staticmesh_component(game& game, entity& owner)
    : reflective(game, owner) {
        game_.rendering_system().register_component(this);
    }

    void staticmesh_component::register_reflection() {
        register_property<std::string>("mesh", nullptr, &staticmesh_component::load);
    }
}