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

    if(gamepad != nullptr) {
        register_config("gamepad", gamepad.get());
    }
}

Json::Value zombye::config_system::get(std::string file, std::string value) {
    auto it = configs_.find(file);

    if(it != configs_.end()) {
        return configs_.at(file)[value];
    }

    return Json::Value();
}

void zombye::config_system::register_config(std::string name, zombye::asset *asset) {
    Json::Reader reader;

    configs_.insert(std::make_pair(name, Json::Value()));

    reader.parse(&(*asset->content().begin()), &(*asset->content().end()), configs_.at(name));
}
