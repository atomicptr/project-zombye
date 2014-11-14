#ifndef __ZOMBYE_CONFIG_SYSTEM_HPP__
#define __ZOMBYE_CONFIG_SYSTEM_HPP__

#include <unordered_map>
#include <utility>
#include <memory>

#include <json/json.h>

#include <zombye/assets/asset_manager.hpp>
#include <zombye/assets/asset.hpp>

#include <zombye/utils/os.h>

namespace zombye {
    class config_system {
    public:
        config_system();

        Json::Value get(std::string, std::string);
        bool has(std::string);

    private:
        std::unordered_map<std::string, Json::Value> configs_;

        void register_config(std::string, zombye::asset*);
    };
}

#endif
