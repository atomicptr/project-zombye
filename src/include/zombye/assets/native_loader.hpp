#ifndef __ZOMBYE_NATIVE_LOADER_HPP__
#define __ZOMBYE_NATIVE_LOADER_HPP__

#include <string>
#include <memory>
#include <fstream>

#include <zombye/assets/asset.hpp>
#include <zombye/assets/asset_loader.hpp>

namespace zombye {
    class native_loader : public zombye::asset_loader {
    public:
        native_loader(std::string);

        std::shared_ptr<asset> load(std::string) const;
    private:
        std::string path_;
    };
}

#endif