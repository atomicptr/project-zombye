#include <zombye/core/game.hpp>
#include <zombye/rendering/animation_component.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    void animation_component::register_reflection() {
        register_property<std::string>("rigged_mesh", nullptr, &animation_component::set_rigged_mesh);
        register_property<std::string>("animation", nullptr, &animation_component::set_animation);
    }

    animation_component::animation_component(game& game, entity& owner) noexcept
    : reflective{game, owner} {
        game_.rendering_system().register_component(this);
    }

    animation_component::animation_component(game& game, entity& owner, const std::string& mesh)
    : reflective{game, owner} {
        game_.rendering_system().register_component(this);
        set_rigged_mesh(mesh);
    }

    animation_component::~animation_component() noexcept {
        game_.rendering_system().unregister_component(this);
    }

    void animation_component::draw() const {
        mesh_->vao().bind();
        for (auto i = 0; i < materials_.size(); ++i) {
            materials_[i].color->bind(0);
            mesh_->draw(i);
        }
    }

    void animation_component::set_rigged_mesh(const std::string& name) {
        auto& rendering_system = game_.rendering_system();
        mesh_ = rendering_system.get_rigged_mesh_manager().load(name);
        if (!mesh_) {
            throw std::runtime_error("could not load rigged mesh from file " + name);
        }
        auto asset = game_.asset_manager().load(name);
        if (!asset) {
            throw std::runtime_error("could not load asset " + name);
        }
        auto data_ptr = asset->content().data();
        auto size = *reinterpret_cast<const size_t*>(data_ptr) * sizeof(bone);
        data_ptr += sizeof(size_t);
        data_ptr += size;
        size = *reinterpret_cast<const size_t*>(data_ptr) * sizeof(skinned_vertex);
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
            auto specular = texture_manager.load("texture/" + name + "_specular.dds");
            if (!color) {
                throw std::runtime_error("could not load texture " + name + "_specular.dds");
            }
            m.specular = specular;
            materials_.emplace_back(m);
        }
    }

    void animation_component::set_animation(const std::string& name) {
        auto& rendering_system = game_.rendering_system();
        animation_ = rendering_system.get_animation_manager().load(name);
        if (!animation_) {
            throw std::runtime_error("could not load animation from file " + name);
        }
    }
}