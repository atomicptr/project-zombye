#ifndef __ZOMBYE_PROPERTY_TYPES_HPP__
#define __ZOMBYE_PROPERTY_TYPES_HPP__

#include <string>

namespace zombye {
    enum property_types {
        UNKNOWN,
        BOOL,
        INT,
        INT2,
        INT3,
        INT4,
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
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
    template <typename type> property_types property_type<type>::type_id_ = property_types::UNKNOWN;
}

#endif
