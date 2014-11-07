#include <zombye/ecs/entity_template.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    entity_template::entity_template(const std::string& name) noexcept : name_(name) { }

    void entity_template::emplace_back(value_pack* value_pack) {
        value_packs_.emplace_back(std::unique_ptr<zombye::value_pack>(value_pack));
    }
}