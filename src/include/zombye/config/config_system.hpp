#ifndef __ZOMBYE_CONFIG_SYSTEM_HPP__
#define __ZOMBYE_CONFIG_SYSTEM_HPP__

#include <unordered_map>
#include <utility>

#include <json/json.h>

#include <zombye/assets/asset_manager.hpp>

namespace zombye {
    class config_system {
    public:
        config_system();

        Json::Value get(std::string, std::string);

    private:
        std::unordered_map<std::string, Json::Value> configs_;
    };
}

#endif
