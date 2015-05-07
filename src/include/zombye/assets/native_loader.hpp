#ifndef __ZOMBYE_NATIVE_LOADER_HPP__
#define __ZOMBYE_NATIVE_LOADER_HPP__

#include <string>
#include <memory>
#include <fstream>

namespace zombye {
    class asset;
    class asset_loader;
}

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
