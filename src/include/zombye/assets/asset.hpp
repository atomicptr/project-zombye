#ifndef __ZOMBEYE_ASSET_HPP__
#define __ZOMBEYE_ASSET_HPP__

#include <string>
#include <vector>

namespace zombye {
    class asset {
    public:
        asset(std::string, size_t);

        asset(const asset&) = delete;
        asset(asset&&) = delete;
        asset& operator=(const asset&) = delete;
        asset& operator=(asset&&) = delete;

        std::string path() const;
        std::vector<char>& content();
    private:
        std::string path_;
        std::vector<char> content_;
    };
}

#endif