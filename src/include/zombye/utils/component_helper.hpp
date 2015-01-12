#ifndef __ZOMBYE_COMPONENT_HELPER_HPP__
#define __ZOMBYE_COMPONENT_HELPER_HPP__

#include <algorithm>
#include <vector>

namespace zombye {
    template <typename type>
    void remove(std::vector<type>& storage, type component) {
        auto it = std::find(storage.begin(), storage.end(), component);
        assert(it != storage.end());
        auto last = storage.end() - 1;
        if (it != last) {
            *it = std::move(*last);
        }
        storage.pop_back();
    }
}

#endif
