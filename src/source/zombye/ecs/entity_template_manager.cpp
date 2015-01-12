#include <zombye/ecs/entity_template_manager.hpp>
#include <zombye/core/game.hpp>
#include <zombye/utils/assign.hpp>

using namespace Json;

namespace zombye {
    entity_template_manager::entity_template_manager(game& game) : game_(game) { }

    std::shared_ptr<const entity_template> entity_template_manager::load_new(const std::string& name,
        const std::string& file) {
        static zombye::asset_manager asset_manager;
        auto asset = asset_manager.load(file);
        if (!asset) {
            return nullptr;
        }
        Json::Value root;
        if (!reader_.parse(&(*asset->content().begin()), &(*asset->content().end()), root)) {
            log(LOG_ERROR, "could not parse " + file + " " + reader_.getFormattedErrorMessages());
            return nullptr;
        }
        auto entity_type = root[name];
        if (entity_type.isNull()) {
            log(LOG_ERROR, "no template " + name + " in " + file);
            return nullptr;
        }
        if (!entity_type.isObject()) {
            log(LOG_ERROR, "template " + name + " is not an object");
            return nullptr;
        }
        auto entity_template = std::make_shared<zombye::entity_template>(name);
        for (auto it = entity_type.begin(); it != entity_type.end(); ++it) {
            auto name = it.key().asString();
            auto rtti = rtti_manager::type_info(name);
            if (!rtti) {
                log(LOG_ERROR, name + " has no runtime type information");
                return nullptr;
            }
            auto value_pack = assign_values(name, *it, rtti->properties());
            if (!value_pack) {
                log(LOG_ERROR, "values could not be assigned to " + name + " value pack");
                return nullptr;
            }
            entity_template->emplace_back(std::move(value_pack));
        }
        return entity_template;
    }
}
