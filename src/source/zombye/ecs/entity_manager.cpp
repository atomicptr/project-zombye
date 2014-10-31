#include <zombye/core/game.hpp>
#include <zombye/ecs/entity_manager.hpp>


namespace zombye {
    entity_manager::entity_manager(game& game) noexcept : game_(game) { }

    zombye::entity& entity_manager::emplace(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scalation) {
        auto entity = new zombye::entity(game_, position, rotation, scalation);
        entities_.insert(std::make_pair(entity->id(), std::unique_ptr<zombye::entity>(entity)));
        return *entity;
    }

    void entity_manager::erase(unsigned long id) {
        auto entity = entities_.find(id);
        if (entity != entities_.end()) {
            deletion_.push(id);
        } else {
            log(LOG_WARNING, "entity " + std::to_string(id) + "doesn't exist");
        }
    }

    void entity_manager::clear() {
        while (!deletion_.empty()) {
            unsigned int kill = deletion_.front();
            auto entity = entities_.find(kill);
            if (entity != entities_.end()) {
                entities_.erase(entity);
                deletion_.pop();
            } else {
                log(LOG_WARNING, "entity " + std::to_string(kill) + " was already destroyed");
            }
        }
    }

    zombye::entity* entity_manager::entity(unsigned long id) noexcept {
        auto entity = entities_.find(id);
        if (entity != entities_.end()) {
            return entity->second.get();
        }
        return nullptr;
    }
}