#ifndef __ZOMBYE_JSON_TYPE_HELPER_HPP__
#define __ZOMBYE_JSON_TYPE_HELPER_HPP__

#include <json/json.h>

namespace zombye {
    template <Json::ValueType, typename type>
    type get(const Json::Value& value) {
        return type();
    }
}

#endif