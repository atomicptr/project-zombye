#ifndef __ZOMBYE_CACHED_RESOURCE_MANAGER_HPP__
#define __ZOMBYE_CACHED_RESOURCE_MANAGER_HPP__

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace zombye {
    template <typename resource, typename manager>
    class cached_resource_manager {
        std::unordered_map<std::string, std::weak_ptr<resource>> cache_;
        std::vector<std::string> clear_buffer_;
    public:
        cached_resource_manager() noexcept = default;
        cached_resource_manager(const cached_resource_manager& other) = delete;
        cached_resource_manager(cached_resource_manager&& other) = delete;
        ~cached_resource_manager() noexcept = default;

        template <typename... arguments>
        std::shared_ptr<resource> load(const std::string& name, arguments... args) {
            if (name.empty()) {
                return nullptr;
            }
            auto it = cache_.find(name);
            if (it != cache_.end()) {
                auto ptr = it->second.lock();
                if (ptr) {
                    return ptr;
                }
            }
            auto ptr = static_cast<manager*>(this)->load_new(name, std::forward<arguments>(args)...);
            if (ptr) {
                cache_[name]=ptr;
                return ptr;
            }
            return nullptr;
        }
        void clear() {
            for (auto& entry : cache_) {
                if (entry.second.expired()) {
                    clear_buffer_.push_back(entry.first);
                }
            }
            for (auto& entry : clear_buffer_) {
                cache_.erase(entry);
            }
            clear_buffer_.clear();
        }

        cached_resource_manager& operator= (const cached_resource_manager& other) = delete;
        cached_resource_manager& operator= (cached_resource_manager&& other) = delete;
    };
}

#endif