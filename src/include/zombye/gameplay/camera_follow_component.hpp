#ifndef __ZOMBYE_CAMERA_FOLLOW_COMPONENT_HPP__
#define __ZOMBYE_CAMERA_FOLLOW_COMPONENT_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class entity;
    class game;
}

namespace zombye {
    class camera_follow_component : public reflective<camera_follow_component, component> {
        friend class reflective<camera_follow_component, component>;
    private:
        unsigned long long target_;
        float elevation_;
        float azimuth_;
        float distance_;
        float min_distance_;
        float max_distance_;
        float spring_constant_;
        float mass_;
        glm::vec3 velocity_;

    public:
        camera_follow_component(game& game, entity& owner, unsigned long target = 0,
            float elevation = glm::pi<float>() * 1.1f, float azimuth = glm::pi<float>(), float distance = 10.f,
            float min_distance = 8.f, float max_distance = 20.f, float spring_constant = 20.f, float mass = 0.5f);
        ~camera_follow_component();

        void update(float delta_time);

        auto target() const {
            return target_;
        }

        void target(unsigned long long target) {
            target_ = target;
        }

        auto elevation() const {
            return elevation_;
        }

        void elevation(float elevation) {
            elevation_ = elevation;
        }

        auto azimuth() const {
            return azimuth_;
        }

        void azimuth(float azimuth) {
            azimuth_ = azimuth;
        }

        auto distance() const {
            return distance_;
        }

        void distance(float distance) {
            distance_ = distance;
        }

        auto min_distance() const {
            return min_distance_;
        }

        void min_distance(float min_distance) {
            min_distance_ = min_distance;
        }

        auto max_distance() const {
            return max_distance_;
        }

        void max_distance(float max_distance) {
            max_distance_ = max_distance;
        }

        auto spring_constant() const {
            return spring_constant_;
        }

        void spring_constant(float spring_constant) {
            spring_constant_ = spring_constant;
        }

        auto mass() const {
            return mass_;
        }

        void mass(float mass) {
            mass_ = mass;
        }

        void first_position(const glm::vec3& offset = glm::vec3{0.f});

        static void register_at_script_engine(game& game);
    };
}

#endif
