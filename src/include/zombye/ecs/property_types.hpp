#ifndef __ZOMBYE_PROPERTY_TYPES_HPP__
#define __ZOMBYE_PROPERTY_TYPES_HPP__

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

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
    template <> property_types property_type<bool>::type_id_ = property_types::BOOL;
    template <> property_types property_type<int>::type_id_ = property_types::INT;
    template <> property_types property_type<glm::ivec2>::type_id_ = property_types::INT2;
    template <> property_types property_type<glm::ivec3>::type_id_ = property_types::INT3;
    template <> property_types property_type<glm::ivec4>::type_id_ = property_types::INT4;
    template <> property_types property_type<float>::type_id_ = property_types::FLOAT;
    template <> property_types property_type<glm::vec2>::type_id_ = property_types::FLOAT2;
    template <> property_types property_type<glm::vec3>::type_id_ = property_types::FLOAT3;
    template <> property_types property_type<glm::vec4>::type_id_ = property_types::FLOAT4;
    template <> property_types property_type<glm::quat>::type_id_ = property_types::QUAT;
    template <> property_types property_type<std::string>::type_id_ = property_types::STRING;
}

#endif
