#include <zombye/ecs/entity_template.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    entity_template::entity_template(const std::string& name) noexcept : name_(name) { }

    void entity_template::push_back(value_pack* value_pack) {
        if (!value_pack) {
            log(LOG_WARNING, "value pack " + std::to_string(value_packs_.size() + 1)
                + " in entity template " + name_ + " is null");
            return;
        }
        value_packs_.push_back(std::unique_ptr<zombye::value_pack>(value_pack));
    }
}