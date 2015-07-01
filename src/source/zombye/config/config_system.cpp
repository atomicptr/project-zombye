#include <fstream>

#include <zombye/assets/asset.hpp>
#include <zombye/assets/asset_loader.hpp>
#include <zombye/assets/asset_manager.hpp>
#include <zombye/config/config_system.hpp>

zombye::config_system::config_system() {
    asset_manager manager;

    auto main = manager.load("config/main.json");

    #ifdef OS_MAC_OS_X
    auto gamepad = manager.load("config/gamepad.osx.json");
    #elif defined OS_LINUX
    auto gamepad = manager.load("config/gamepad.linux.json");
    #elif defined OS_WINDOWS
    auto gamepad = manager.load("config/gamepad.windows.json");
    #else
    #error "UNKNOWN OS"
    #endif

    register_config("main", main.get());

    auto quality = manager.load("config/quality.json");
    register_config("quality", quality.get());

    if(gamepad != nullptr) {
        register_config("gamepad", gamepad.get());
    }
}

Json::Value zombye::config_system::get(std::string file, std::string ident) {
    auto it = configs_.find(file);

    if(it != configs_.end()) {
        return configs_.at(file)[ident];
    }

    return Json::Value();
}

void zombye::config_system::set(std::string file, std::string ident, Json::Value value) {
    auto it = configs_.find(file);

    if(it != configs_.end()) {
        configs_.at(file)[ident] = value;
    }
}

bool zombye::config_system::has(std::string file) {
    auto it = configs_.find(file);

    return it != configs_.end();
}

void zombye::config_system::commit() {
    for(auto &c : configs_) {
        commit(c.first);
    }
}

void zombye::config_system::commit(std::string file) {
    auto it = configs_.find(file);

    if(it != configs_.end()) {
        auto ident = (*it).first;
        auto json = (*it).second;

        auto str = json.toStyledString();

        auto path = paths_.at(ident);

        std::ofstream file;

        file.open(std::string{"assets/"} + path, std::ios::out | std::ios::trunc);

        file << str;

        file.close();
    }
}

void zombye::config_system::register_config(std::string name, zombye::asset *asset) {
    Json::Reader reader;

    configs_.insert(std::make_pair(name, Json::Value()));
    paths_.insert(std::make_pair(name, asset->path()));

    reader.parse(&(*asset->content().begin()), &(*asset->content().end()), configs_.at(name));
}
