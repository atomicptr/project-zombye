#ifndef __ZOMBYE_ENTITY_MANAGER_HPP__
#define __ZOMBYE_ENTITY_MANAGER_HPP__

#include <memory>
#include <queue>
#include <unordered_map>

#include <zombye/ecs/entity.hpp>
#include <zombye/ecs/entity_template_manager.hpp>

namespace zombye {
    class game;
    class entity_manager {
        game& game_;
        std::unordered_map<unsigned long, std::unique_ptr<entity>> entities_;
        std::queue<unsigned long> deletion_;
        entity_template_manager template_manager_;
    public:
        entity_manager(game& game) noexcept;
        entity_manager(const entity_manager& other) = delete;
        entity_manager(entity_manager&& other) = delete;
        zombye::entity& emplace(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scalation);
        zombye::entity& emplace(const std::string& name, const glm::vec3& position, const glm::quat& rotation,
            const glm::vec3& scalation);

        void erase(unsigned long id);
        void clear();
        entity* resolve(unsigned long id) noexcept;
        entity_manager& operator= (const entity_manager& other) = delete;
        entity_manager& operator= (entity_manager&& other) = delete;
    };
}

#endif