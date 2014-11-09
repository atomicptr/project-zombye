#ifndef __ZOMBYE_TEST_COMPONENT_HPP__
#define __ZOMBYE_TEST_COMPONENT_HPP__

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class game;
    class entity;
    class test_component : public reflective<test_component, component> {
        friend class reflective<test_component, component>;
        bool boolean_;
        int integer_;
        glm::ivec2 ivec2_;
        glm::ivec3 ivec3_;
        glm::ivec4 ivec4_;
        float floating_point_;
        glm::vec2 vec2_;
        glm::vec3 vec3_;
        glm::vec4 vec4_;
        glm::quat quat_;
        std::string string_;
    public:
        test_component(game& game, entity& owner)
        : reflective(game, owner), boolean_{}, integer_{}, ivec2_{}, ivec3_{}, ivec4_{},
        floating_point_{}, vec2_{}, vec3_{}, vec4_{}, quat_{}, string_{} { }

        bool boolean() const {
            return boolean_;
        }

        int integer() const {
            return integer_;
        }

        glm::ivec2 ivec2() const {
            return ivec2_;
        }

        glm::ivec3 ivec3() const {
            return ivec3_;
        }

        glm::ivec4 ivec4() const {
            return ivec4_;
        }

        float floating_point() const {
            return floating_point_;
        }

        glm::vec2 vec2() const {
            return vec2_;
        }

        glm::vec3 vec3() const {
            return vec3_;
        }

        glm::vec4 vec4() const {
            return vec4_;
        }

        glm::quat quat() const {
            return quat_;
        }

        std::string string() const {
            return string_;
        }

        void set_boolean(const bool& boolean) {
            boolean_ = boolean;
        }

        void set_integer(const int& integer) {
            integer_ = integer;
        }

        void set_ivec2(const glm::ivec2& ivec2) {
            ivec2_ = ivec2;
        }

        void set_ivec3(const glm::ivec3& ivec3) {
            ivec3_ = ivec3;
        }

        void set_ivec4(const glm::ivec4& ivec4) {
            ivec4_ = ivec4;
        }

        void set_floating_point(const float& floating_point) {
            floating_point_ = floating_point;
        }

        void set_vec2(const glm::vec2& vec2) {
            vec2_ = vec2;
        }

        void set_vec3(const glm::vec3& vec3) {
            vec3_ = vec3;
        }

        void set_vec4(const glm::vec4& vec4) {
            vec4_ = vec4;
        }

        void set_quat(const glm::quat& quat) {
            quat_ = quat;
        }

        void set_string(const std::string& string) {
            string_ = string;
        }
    private:
        static void register_reflection() {
            register_property<bool>("boolean", &test_component::boolean, &test_component::set_boolean);
            register_property<int>("integer", &test_component::integer, &test_component::set_integer);
            register_property<glm::ivec2>("ivec2", &test_component::ivec2, &test_component::set_ivec2);
            register_property<glm::ivec3>("ivec3", &test_component::ivec3, &test_component::set_ivec3);
            register_property<glm::ivec4>("ivec4", &test_component::ivec4, &test_component::set_ivec4);
            register_property<float>("floating_point", &test_component::floating_point,
                &test_component::set_floating_point);
            register_property<glm::vec2>("vec2", &test_component::vec2, &test_component::set_vec2);
            register_property<glm::vec3>("vec3", &test_component::vec3, &test_component::set_vec3);
            register_property<glm::vec4>("vec4", &test_component::vec4, &test_component::set_vec4);
            register_property<glm::quat>("quat", &test_component::quat, &test_component::set_quat);
            register_property<std::string>("string", &test_component::string, &test_component::set_string);
        }
    };
}

#endif