#ifndef __ZOMBYE_ENTITY_TEMPLATE_MANAGER_HPP__
#define __ZOMBYE_ENTITY_TEMPLATE_MANAGER_HPP__

#include <memory>
#include <string>
#include <unordered_map>

#include <json/json.h>

#include <zombye/utils/cached_resource_manager.hpp>
#include <zombye/ecs/entity_template.hpp>

namespace zombye {
    class game;
    class entity_template_manager : public cached_resource_manager<const entity_template, entity_template_manager> {
        game& game_;
        Json::Reader reader_;
    public:
        entity_template_manager(game& game);
        ~entity_template_manager() = default;
        friend class cached_resource_manager<const entity_template, entity_template_manager>;
    protected:
        std::shared_ptr<const entity_template> load_new(const std::string& name, const std::string& file);
    private:
        value_pack* assign_values(const std::string& name, const Json::Value& object,
        const std::vector<std::unique_ptr<abstract_property>>& properties);
    };
}

#endif