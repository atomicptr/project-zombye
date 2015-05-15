#include <stack>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

#include <zombye/core/game.hpp>
#include <zombye/rendering/animation_component.hpp>
#include <zombye/rendering/animation_system.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/staticmesh_component.hpp>
#include <zombye/rendering/skinned_mesh.hpp>
#include <zombye/rendering/skeleton.hpp>
#include <zombye/rendering/texture.hpp>
#include <zombye/utils/logger.hpp>

#include <glm/gtx/string_cast.hpp>

namespace zombye {
    animation_component::animation_component(game& game, entity& owner, const std::string& mesh, const std::string& skeleton)
    : reflective{game, owner}, current_state_{""}, elapsed_time_{0.f} {
        game_.rendering_system().register_component(this);
        game_.animation_system().register_component(this);
        load(mesh);
        load_skeleton(skeleton);
    }

    animation_component::~animation_component() noexcept {
        game_.rendering_system().unregister_component(this);
        game_.animation_system().unregister_component(this);
    }

    void animation_component::update(float delta_time) {
        static std::vector<glm::mat4> pose(skeleton_->bones().size(), glm::mat4{1.f});
        static const float fps = 1.f / 24.f;
        pose.assign(skeleton_->bones().size(), glm::mat4{1.f});

        if (current_state_ != "") {
            auto& animation = skeleton_->animation(current_state_);
            auto& bones = skeleton_->bones();
            auto& bone_hierachy = skeleton_->bone_hierachy_;
            auto length = animation.length * fps;

            elapsed_time_ += delta_time;
            if (elapsed_time_ <= length) {
                std::stack<int> traversal;
                traversal.push(0);
                while(!traversal.empty()) {
                    auto i = traversal.top();
                    traversal.pop();

                    auto& track = animation.tracks.at(i);

                    auto& current_frame = current_keyframes_.at(i);
                    auto next_frame = current_frame;
                    ++next_frame.tkey;
                    ++next_frame.qkey;
                    ++next_frame.skey;

                    auto iv = interpolate_translation_keyframe(track, current_frame, next_frame, fps);

                    auto iq = interpolate_quaternion_keyframe(track, current_frame, next_frame, fps);

                    auto p = glm::toMat4(iq);
                    p[3].x = iv.x;
                    p[3].y = iv.y;
                    p[3].z = iv.z;

                    pose[i] *= bones.at(i).relative_transform * p;
                    if (bone_hierachy.find(i) != bone_hierachy.end()) {
                        for (auto node = bone_hierachy.begin(i); node != bone_hierachy.end(i); ++node) {
                            auto& child = node->second;
                            pose[child] *= pose[i];
                            traversal.push(child);
                        }
                    }
                }
            } else {
                elapsed_time_ = 0.f;
                for (auto& k : current_keyframes_) {
                    k.tkey = 0;
                    k.qkey = 0;
                    k.skey = 0;
                }
                return;
            }
        } else {
            auto& bones = skeleton_->bones();
            for (auto i = 0u; i < bones.size(); ++i) {
                pose[i] = glm::inverse(bones.at(i).absolute_transform);
            }
        }

        auto& bones = skeleton_->bones();
        for (auto i = 0; i < pose_.size(); ++i) {
            pose_[i] = pose[i] * bones.at(i).absolute_transform;
        }
    }

    void animation_component::draw() const noexcept {
        mesh_->draw();
    }

    void animation_component::load(const std::string& mesh) {
        auto& rendering_system = game_.rendering_system();

        mesh_ = rendering_system.skinned_mesh_manager().load(mesh);
        if (!mesh_) {
            log(LOG_FATAL, "could not load mesh from file " + mesh);
        }
    }

    void animation_component::load_skeleton(const std::string& skeleton) {
        skeleton_ = game_.rendering_system().skeleton_manager().load(skeleton);
        if (!skeleton_) {
            log(LOG_FATAL, "could not load skeleton " + skeleton);
        }

        auto& bones = skeleton_->bones();
        for (auto i = 0u; i < bones.size(); ++i) {
            pose_.emplace_back(bones[i].absolute_transform);
        }

        current_keyframes_.resize(bones.size());
    }

    glm::vec3 animation_component::interpolate_translation_keyframe(const track& track, frame& current_frame, frame& next_frame, float fps) {
        auto t1 = track.tkeys[current_frame.tkey].time * fps;
        auto t2 = track.tkeys[next_frame.tkey].time * fps;
        if (elapsed_time_ >= t2 && next_frame.tkey < track.tkeys.size() - 1) {
            ++current_frame.tkey;
            ++next_frame.tkey;
            t1 = track.tkeys[current_frame.tkey].time * fps;
            t2 = track.tkeys[next_frame.tkey].time * fps;
        }

        auto v1 = track.tkeys[current_frame.tkey].translate;
        auto v2 = track.tkeys[next_frame.tkey].translate;

        auto delta = (elapsed_time_ - t1) / (t2 - t1);
        delta = delta > 1.f ? 1.f : delta;

        return glm::lerp(v1, v2, delta);
    }

    glm::quat animation_component::interpolate_quaternion_keyframe(const track& track, frame& current_frame, frame& next_frame, float fps) {
        auto t1 = track.qkeys[current_frame.qkey].time * fps;
        auto t2 = track.qkeys[next_frame.qkey].time * fps;
        if (elapsed_time_ >= t2 && next_frame.qkey < track.qkeys.size() - 1) {
            ++current_frame.qkey;
            ++next_frame.qkey;
            t1 = track.qkeys[current_frame.qkey].time * fps;
            t2 = track.qkeys[next_frame.qkey].time * fps;
        }

        auto q1 = track.qkeys[current_frame.qkey].rotate;
        q1 = glm::normalize(q1);
        auto q2 = track.qkeys[next_frame.qkey].rotate;
        q2 = glm::normalize(q2);

        auto delta = (elapsed_time_ - t1) / (t2 - t1);
        delta = delta > 1.f ? 1.f : delta;

        return glm::normalize(glm::lerp(q1, q2, delta));
    }

    animation_component::animation_component(game& game, entity& owner)
    : reflective(game, owner), current_state_{""}, elapsed_time_{0.f} {
        game_.rendering_system().register_component(this);
        game_.animation_system().register_component(this);
    }

    void animation_component::register_reflection() {
        register_property<std::string>("mesh", nullptr, &animation_component::load);
        register_property<std::string>("skeleton", nullptr, &animation_component::load_skeleton);
    }
}
