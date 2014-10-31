#ifndef __ZOMBYE_ENTITY_HPP__
#define __ZOMBYE_ENTITY_HPP__

#include <memory>
#include <stdexcept>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/rtti_manager.hpp>

namespace zombye {
    class game;
    class entity {
    protected:
        static unsigned long next_id_;
        game& game_;
        unsigned long id_;
        std::unordered_map<unsigned long, std::unique_ptr<class component>> components_;
        glm::vec3 position_;
        glm::quat rotation_;
        glm::vec3 scalation_;

        template <size_t i = 0, typename... arguments>
        void fill_in_properties(zombye::component* owner) { }

        template <size_t i = 0, typename arg, typename... arguments>
        void fill_in_properties(zombye::component* owner, arg first, arguments... args) {
            auto type_info = owner->rtti();
            if (type_info) {
                static_cast<typed_property<arg>*>(type_info->properties()[i].get())->set_value(owner, first);
            }
            fill_in_properties<i + 1, arguments...>(owner, args...);
        }
    public:
        entity(game& game, glm::vec3 position, glm::quat rotation, glm::vec3 scalation) noexcept;
        entity(const entity& other) = delete;
        entity(entity&& other) = delete;
        ~entity() noexcept = default;

        template <typename component_type, typename... arguments>
        component_type& emplace(arguments... args) {
            auto type_info = component_type::type_rtti();
            if (!type_info) {
                log(LOG_ERROR, demangle(typeid(component_type).name()) + " has no runtime type information");
                throw std::invalid_argument(demangle(typeid(component_type).name()) + " has no runtime type information");
            }
            auto insert_position = components_.find(type_info->type_id());
            if (insert_position == components_.end()) {
                auto component = new component_type(game_, *this, std::forward<arguments>(args)...);
                components_.insert(std::make_pair(type_info->type_id(), std::unique_ptr<component_type>(component)));
                return *component;
            } else {
                log(LOG_WARNING, "entity " + std::to_string(id_) + " already has component of type " + type_info->type_name());
                return *static_cast<component_type*>(insert_position->second.get());
            }
        }

        template <typename... arguments>
        zombye::component& emplace(const std::string& name, arguments... args) {
            auto type_info = rtti_manager::type_info(name);
            if (!type_info) {
                log(LOG_ERROR, name + " has no runtime type information");
                throw std::invalid_argument(name + " has no runtime type information");
            }
            auto insert_position = components_.find(type_info->type_id());
            if (insert_position == components_.end()) {
                auto component = type_info->factory()(game_, *this);
                components_.insert(std::make_pair(type_info->type_id(), std::unique_ptr<zombye::component>(component)));
                fill_in_properties(component, args...);
                return *component;
            } else {
                log(LOG_WARNING, "entity " + std::to_string(id_) + " already has component of type " + type_info->type_name());
                return *(insert_position->second.get());
            }
        }

        template <typename component_type>
        void erase() {
            auto type_info = component_type::type_rtti();
            if (!type_info) {
                log(LOG_ERROR, demangle(typeid(component_type).name()) + " has no runtime type information");
                throw std::invalid_argument(demangle(typeid(component_type).name()) + " has no runtime type information");
            }
            auto erase_position = components_.find(type_info->type_id());
            if (erase_position != components_.end()) {
                components_.erase(erase_position);
            } else {
                log(LOG_WARNING, "entity " + std::to_string(id_) + " has no component of type " + type_info->type_name());
            }
        }

        template <typename component_type>
        component_type* component() noexcept {
            auto type_info = component_type::type_rtti();
            if (!type_info) {
                log(LOG_ERROR, demangle(typeid(component_type).name()) + " has no runtime type information");
                throw std::invalid_argument(demangle(typeid(component_type).name()) + " has no runtime type information");
            }
            auto component_position = components_.find(type_info->type_id());
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
