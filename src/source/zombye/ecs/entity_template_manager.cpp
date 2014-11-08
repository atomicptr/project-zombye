#include <zombye/ecs/entity_template_manager.hpp>
#include <zombye/core/game.hpp>

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
            log(LOG_ERROR, "Could not parse " + file + " " + reader_.getFormattedErrorMessages());
            return nullptr;
        }
        auto entity_type = root[name];
        if (entity_type.isNull()) {
            log(LOG_ERROR, "No template " + name + " in " + file);
            return nullptr;
        }
        if (!entity_type.isObject()) {
            log(LOG_ERROR, "Template " + name + " is not an object");
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
            /*
            auto value_pack = new zombye::value_pack(name);
            for (auto& p : rtti->properties()) {
                auto value = (*it)[p->name()];
                auto type = value.type();
                switch (type) {
                    case ValueType::booleanValue:
                        if (p->type() != property_types::BOOL) {
                            log(LOG_ERROR, p->name() + " has wrong type in template");
                            return nullptr;
                        }
                        value_pack->emplace_back(*static_cast<typed_property<bool>*>(p.get()), value.asBool());
                        break;
                    case ValueType::intValue:
                        if (p->type() != property_types::INT) {
                            log(LOG_ERROR, p->name() + " has wrong type in template");
                            return nullptr;
                        }
                        value_pack->emplace_back(*static_cast<typed_property<int>*>(p.get()), value.asInt());
                        break;
                    case ValueType::realValue:
                        if (p->type() != property_types::FLOAT) {
                            log(LOG_ERROR, p->name() + " has wrong type in template");
                            return nullptr;
                        }
                        value_pack->emplace_back(*static_cast<typed_property<float>*>(p.get()), value.asFloat());
                        break;
                    case ValueType::stringValue:
                        if (p->type() != property_types::STRING) {
                            log(LOG_ERROR, p->name() + " has wrong type in template");
                            return nullptr;
                        }
                        value_pack->emplace_back(*static_cast<typed_property<std::string>*>(p.get()), value.asString());
                    case ValueType::arrayValue:
                        break;
                    default:
                        break;
                }
            }*/
            auto value_pack = assign_values(name, *it, rtti->properties());
            if (!value_pack) {
                log(LOG_ERROR, "Values could not be assigned to " + name + " value pack");
                return nullptr;
            }
            entity_template->emplace_back(value_pack);
        }
        return entity_template;
    }

    zombye::value_pack* entity_template_manager::assign_values(const std::string& name, const Json::Value& object,
    const std::vector<std::unique_ptr<abstract_property>>& properties) {
        auto value_pack = new zombye::value_pack(name);
        for (auto& p : properties) {
            auto wrapped_value = object[p->name()];
            auto type = wrapped_value.type();
            //auto value = get<wrapped_value.type()>(wrapped_value);
            //value_pack->emplace_back(*static_cast<typed_property<decltype(value)>*>(p.get()), value);
        }
        return value_pack;
    }
}