#ifndef __ZOMBYE_COLLECTION_HPP__
#define __ZOMBYE_COLLECTION_HPP__

#include <map>
#include <string>
#include <utility>

#include <zombye/assets/asset_manager.hpp>
#include <zombye/utils/audiohelper.hpp>

namespace zombye {

    template<typename T>
    class collection {
    public:
        collection() {
        }

        T* get(std::string name) {
            return files_[name].get();
        }

        void add(std::string name, std::string asset_path) {
            static asset_manager manager;

            files_.insert(std::make_pair(name, std::unique_ptr<T>(new T(asset_path))));
        }
    private:
        std::map<std::string, std::unique_ptr<T>> files_;
    };
}

#endif