#ifndef __ZOMBEYE_ASSET_MANAGER_HPP__
#define __ZOMBEYE_ASSET_MANAGER_HPP__

#include <vector>
#include <memory>
#include <string>

#include <zombye/assets/asset_loader.hpp>
#include <zombye/assets/asset.hpp>

#include <zombye/assets/native_loader.hpp>

#include <zombye/utils/logger.hpp>

namespace zombye {
    class asset_manager {
    public:
        asset_manager();
        ~asset_manager() = default;

        asset_manager(const asset_manager&) = delete;
        asset_manager(asset_manager&&) = delete;
        asset_manager& operator=(const asset_manager&) = delete;
        asset_manager& operator=(asset_manager&&) = delete;

        std::shared_ptr<asset> load(std::string) const;
    private:
        std::vector<std::unique_ptr<asset_loader>> loaders_;
    };
}

#endif