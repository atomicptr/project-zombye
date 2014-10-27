#ifndef __ZOMBYE_ENTITY_HPP__
#define __ZOMBYE_ENTITY_HPP__

#include <memory>
#include <stdexcept>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <zombye/ecs/component.hpp>

namespace zombye {
    class game;
    class entity {
    protected:
        static unsigned long next_id_;
        game& game_;
        unsigned long id_;
        std::unordered_map<unsigned int, std::unique_ptr<class component>> components_;
        glm::vec3 position_;
        glm::quat rotation_;
        glm::vec3 scalation_;
    public:
        entity(game& game, glm::vec3 position, glm::quat rotation, glm::vec3 scalation) noexcept;
        entity(const entity& other) = delete;
        entity(entity&& other) = delete;
        ~entity() noexcept = default;

        template <typename component_type, typename... arguments>
        component_type& emplace(arguments... args) {
            auto insert_position = components_.find(component_type::rtti::type_id);
            if (insert_position == components_.end()) {
                auto component = new component_type(game_, *this, std::forward<arguments>(args)...);
                components_.insert(std::make_pair(component_type::rtti::type_id, std::unique_ptr<component_type>(component)));
                return *component;
            } else {
                // TODO: Appropirate error handling.
            }
        }

        template <typename component_type>
        void erase() {
            auto erase_position = components_.find(component_type::rtti::type_id);
            if (erase_position != components_.end()) {
                components_.erase(erase_position);
            } else {
                // TODO: Appropirate error handling.
            }
        }

        template <typename component_type>
        component_type* component() noexcept {
            auto component_position = components_.find(component_type::rtti::type_id);
            if (component_position != components_.end()) {
                return static_cast<component_type*>(component_position->second.get());
            }
            return nullptr;
        }

        unsigned long id() const noexcept {
            return id_;
        }

        const glm::vec3& position() const noexcept {
            return position_;
        }

        void position(const glm::vec3& position) noexcept {
            position_ = position;
        }

        const glm::quat& rotation() const noexcept {
            return rotation_;
        }

        void rotation(const glm::quat& rotation) noexcept {
            rotation_ = rotation;
        }

        const glm::vec3 scalation() const noexcept {
            return scalation_;
        }

        void scalation(const glm::vec3& scalation) noexcept {
            scalation_ = scalation;
        }

        glm::mat4 transform() const;
    };
}

#endif
