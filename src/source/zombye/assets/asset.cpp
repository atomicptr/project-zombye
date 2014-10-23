#include <zombye/assets/asset.hpp>

zombye::asset::asset(std::string path, size_t size) : path_(path), content_(size) {}

std::string zombye::asset::path() const {
    return path_;
}

std::vector<char>& zombye::asset::content() {
    return content_;
}