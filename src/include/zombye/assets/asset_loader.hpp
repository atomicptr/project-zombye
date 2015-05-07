#ifndef __ZOMBEYE_ASSET_LOADER_HPP__
#define __ZOMBEYE_ASSET_LOADER_HPP__

#include <memory>
#include <string>

namespace zombye {
    class asset;
}

namespace zombye {

    class asset_loader {
    public:
        virtual std::shared_ptr<asset> load(std::string) const = 0;
    };
}

#endif
