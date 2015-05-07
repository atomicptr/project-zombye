#ifndef __ZOMBYE_CONFIG_SYSTEM_HPP__
#define __ZOMBYE_CONFIG_SYSTEM_HPP__

#include <unordered_map>
#include <utility>
#include <memory>
#include <fstream>

#include <json/json.h>

#include <zombye/utils/os.h>

namespace zombye {
    class asset;
}

namespace zombye {
    class config_system {
    public:
        config_system();

        Json::Value get(std::string, std::string);
        void set(std::string, std::string, Json::Value);
        bool has(std::string);

        void commit();
        void commit(std::string);
    private:
        std::unordered_map<std::string, Json::Value> configs_;
        std::unordered_map<std::string, std::string> paths_;

        void register_config(std::string, zombye::asset*);
    };
}

#endif
