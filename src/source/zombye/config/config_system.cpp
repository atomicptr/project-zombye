#include <zombye/config/config_system.hpp>

zombye::config_system::config_system() {
    asset_manager manager;
    Json::Reader reader;

    configs_.insert(std::make_pair("main", Json::Value()));

    auto main = manager.load("config/main.json");

    reader.parse(&(*main->content().begin()), &(*main->content().end()), configs_.at("main"));
}

Json::Value zombye::config_system::get(std::string file, std::string value) {
    auto it = configs_.find(file);

    if(it != configs_.end()) {
        return configs_.at(file)[value];
    }

    return Json::Value();
}
