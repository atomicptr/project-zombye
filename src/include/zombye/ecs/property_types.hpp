#ifndef __ZOMBYE_PROPERTY_TYPES_HPP__
#define __ZOMBYE_PROPERTY_TYPES_HPP__

#include <string>

namespace zombye {
    enum property_types {
        UNKNOWN,
        BOOL,
        INT,
        IVEC2,
        IVEC3,
        IVEC4,
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        QUAT,
        STRING
    };

    template <typename type>
    class property_type {
        static property_types type_id_;
    public:
        static property_types type_id() {
            return type_id_;
        }
    };
    // thanks to gcc there is no default assignment for unknown types
    //template <typename type>property_types property_type<type>::type_id_ = property_types::UNKNOWN;
}

#endif
