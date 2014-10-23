#include <iostream>
#include <string>

#include <zombye/assets/asset_manager.hpp>

int main() {
    std::cout << "init zombye" << std::endl;

    zombye::asset_manager manager;

    auto asset = manager.load("keep_me.txt");

    if(asset) {
        std::cout << std::string(asset->content().begin(), asset->content().end()) << std::endl;
    } else {
        std::cout << "no asset found :(" << std::endl;
    }

    return 0;
}