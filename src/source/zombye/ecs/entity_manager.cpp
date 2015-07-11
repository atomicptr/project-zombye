#include <zombye/core/game.hpp>
#include <zombye/ecs/entity_manager.hpp>
#include <zombye/scripting/scripting_system.hpp>


namespace zombye {
    entity_manager::entity_manager(game& game) noexcept : game_(game), template_manager_(game) {
        auto& scripting_system = game.scripting_system();

        scripting_system.register_type<entity>("entity_impl");

        static std::function<entity&(const glm::vec3&, const glm::quat&, const glm::vec3&)> entity_factory
            = [this](const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scale) -> entity& {
                return emplace(pos, rot, scale);
            };
        scripting_system.register_factory("entity_impl",
            "entity_impl@ f(const glm::vec3& in, const glm::quat& in, const glm::vec3& in)",
            entity_factory
        );

        static std::function<entity*(unsigned long)> resolve_entity = [this](unsigned long id) {
            return resolve(id);
        };
        scripting_system.register_function("entity_impl@ get_entity(uint64 id)", resolve_entity);

        scripting_system.register_member_function("entity_impl", "uint64 id()",
            +[](entity& e) { return e.id(); });
        scripting_system.register_member_function("entity_impl", "const glm::vec3& position() const",
            +[](const entity& e) -> const glm::vec3& { return e.position(); });
        scripting_system.register_member_function("entity_impl", "void position(const glm::vec3& in value)",
            +[](entity& e, const glm::vec3& value) { e.position(value); });
        scripting_system.register_member_function("entity_impl", "const glm::quat& rotation() const",
            +[](const entity& e) -> const glm::quat& { return e.rotation(); });
        scripting_system.register_member_function("entity_impl", "void rotation(const glm::quat& in value)",
            +[](entity& e, const glm::quat& value) { e.rotation(value); });
        scripting_system.register_member_function("entity_impl", "const glm::vec3& scale() const",
            +[](const entity& e) -> const glm::vec3& { return e.scalation(); });
        scripting_system.register_member_function("entity_impl", "void scale(const glm::vec3& in value)",
            +[](entity& e, const glm::vec3& value) { e.scalation(value); });

        static auto player_id = 0ul;
        scripting_system.register_global_object("uint64 player_id", &player_id);
    }

    zombye::entity& entity_manager::emplace(const glm::vec3& position, const glm::quat& rotation,
    const glm::vec3& scalation) {
        auto entity = new zombye::entity(game_, position, rotation, scalation);
        entities_.insert(std::make_pair(entity->id(), std::unique_ptr<zombye::entity>(entity)));
        return *entity;
    }

    zombye::entity& entity_manager::emplace(const std::string& name, const glm::vec3& position,
    const glm::quat& rotation, const glm::vec3& scalation) {
        auto entity = new zombye::entity(game_, position, rotation, scalation);
        entities_.insert(std::make_pair(entity->id(), std::unique_ptr<zombye::entity>(entity)));
        auto entity_template = template_manager_.load(name, "entity_templates.json");
        if (!entity_template) {
            throw std::invalid_argument("no template " + name + " in entity_templates.json");
        }
        for (auto& pack : entity_template->get()) {
            entity->emplace(pack->name(), *pack.get());
        }
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

    entity* entity_manager::resolve(unsigned long id) noexcept {
        auto entity = entities_.find(id);
        if (entity != entities_.end()) {
            return entity->second.get();
        }
        return nullptr;
    }
}
