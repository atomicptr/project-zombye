#include <zombye/core/game.hpp>
#include <zombye/rendering/staticmesh_component.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    void staticmesh_component::register_reflection() {
        register_property<std::string>("mesh", nullptr, &staticmesh_component::set_mesh);
    }

    staticmesh_component::staticmesh_component(game& game, entity& owner) noexcept
    : reflective{game, owner} {
        game_.rendering_system().register_component(this);
    }

    staticmesh_component::staticmesh_component(game& game, entity& owner, const std::string& mesh,
    const std::string& diffuse_texture)
    : reflective{game, owner} {
        auto& rendering_system = game.rendering_system();
        rendering_system.register_component(this);
        mesh_ = rendering_system.get_mesh_manager().load(mesh);
        if (!mesh_) {
            throw std::runtime_error("could not load mesh from file " + mesh);
        }
        auto asset = game_.asset_manager().load(mesh);
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
        auto& texture_manager = rendering_system.get_texture_manager();
        for (auto i = 0; i < material_count; ++i) {
            size = *reinterpret_cast<const size_t*>(data_ptr);
            data_ptr += sizeof(size_t);
            auto name = std::string{data_ptr, size};
            data_ptr += size;
            material m;
            auto color = texture_manager.load("texture/" + name + "_color.dds");
            if (!color) {
                throw std::runtime_error("could not load texture " + name + "_color.dds");
            }
            m.color = color;
            materials_.emplace_back(m);
        }
    }

    staticmesh_component::~staticmesh_component() noexcept {
        game_.rendering_system().unregister_component(this);
    }

    void staticmesh_component::draw() const {
        mesh_->vao().bind();
        for (auto i = 0; i < materials_.size(); ++i) {
            materials_[i].color->bind(0);
            mesh_->draw(i);
        }
    }

    void staticmesh_component::set_mesh(const std::string& name) {
        auto& rendering_system = game_.rendering_system();
        auto tmp = rendering_system.get_mesh_manager().load(name);
        if (tmp) {
            mesh_ = tmp;
        }
        auto asset = game_.asset_manager().load(name);
        if (!asset) {
            throw std::runtime_error("could not load asset " + name);
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
        auto& texture_manager = rendering_system.get_texture_manager();
        for (auto i = 0; i < material_count; ++i) {
            size = *reinterpret_cast<const size_t*>(data_ptr);
            data_ptr += sizeof(size_t);
            auto name = std::string{data_ptr, size};
            data_ptr += size;
            material m;
            auto color = texture_manager.load("texture/" + name + "_color.dds");
            if (!color) {
                throw std::runtime_error("could not load texture " + name + "_color.dds");
            }
            m.color = color;
            materials_.emplace_back(m);
        }
    }
}