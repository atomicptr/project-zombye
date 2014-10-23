#ifndef __ZOMBYE_SMART_HPP__
#define __ZOMBYE_SMART_HPP__

#include <memory>
#include <stdexcept>
#include <utility>

namespace zombye {

    template<typename T, typename Constructor, typename Destructor, typename... Arguments>
    auto make_smart(Constructor construct, Destructor destroy, Arguments... args) {
        auto ptr = construct(std::forward<Arguments>(args)...);

        if(!ptr) {
            throw std::runtime_error("Err: not able to create smart pointer");
        }

        return std::unique_ptr<T, void(*)(T*)>(ptr, destroy);
    }
}

#endif