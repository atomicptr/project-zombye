#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <zombye/ecs/property_types.hpp>

namespace zombye {
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
