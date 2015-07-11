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
#include <zombye/scripting/scripting_system.hpp>
#include <zombye/utils/logger.hpp>

#include <glm/gtx/string_cast.hpp>

namespace zombye {
    animation_component::animation_component(game& game, entity& owner, const std::string& mesh, const std::string& skeleton)
    : reflective{game, owner}, current_state_{""}, elapsed_time_{0.f}, blend_{false}, blend_next_{false} {
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
            auto& bones = skeleton_->bones();
            auto& bone_hierachy = skeleton_->bone_hierachy_;

            elapsed_time_ += delta_time;

            if (blend_ && next_state_ != "") {
                auto& animation = skeleton_->animation(current_state_);
                auto length = animation.length * fps;
                auto& next_animation = skeleton_->animation(next_state_);
                auto next_animation_length = next_animation.length * fps;
                auto delta_factor = next_animation_length <= length ? 1.f / next_animation_length : 1.f / length;

                if (elapsed_time_ <= next_animation_length) {
                    if (elapsed_time_ >= length) {
                        current_state_ = next_state_;
                        next_state_ = "";
                        blend_ = false;
                        current_keyframes_ = next_keyframes_;
                        return;
                    } else {
                        std::stack<int> traversal;
                        traversal.push(0);
                        while(!traversal.empty()) {
                            auto i = traversal.top();
                            traversal.pop();

                            auto& cs_track = animation.tracks.at(i);

                            auto& cs_current_frame = current_keyframes_.at(i);
                            auto cs_next_frame = cs_current_frame;
                            ++cs_next_frame.tkey;
                            ++cs_next_frame.qkey;
                            ++cs_next_frame.skey;

                            auto iv_current = interpolate_translation_keyframe(cs_track, cs_current_frame, cs_next_frame, fps);
                            auto iq_current = interpolate_quaternion_keyframe(cs_track, cs_current_frame, cs_next_frame, fps);

                            auto& ns_track = next_animation.tracks.at(i);

                            auto& ns_current_frame = next_keyframes_.at(i);
                            auto ns_next_frame = ns_current_frame;
                            ++ns_next_frame.tkey;
                            ++ns_next_frame.qkey;
                            ++ns_next_frame.skey;

                            auto iv_next = interpolate_translation_keyframe(ns_track, ns_current_frame, ns_next_frame, fps);
                            auto iq_next = interpolate_quaternion_keyframe(ns_track, ns_current_frame, ns_next_frame, fps);

                            auto delta = elapsed_time_ * delta_factor;
                            delta = delta > 1.f ? 1.f : delta;
                            auto iv = glm::lerp(iv_current, iv_next, delta);
                            auto iq = glm::lerp(iq_current, iq_next, delta);

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
                    }
                } else {
                    elapsed_time_ = 0.f;
                    current_keyframes_.assign(current_keyframes_.size(), frame{});
                    next_keyframes_.assign(next_keyframes_.size(), frame{});
                    current_state_ = next_state_;
                    next_state_ = "";
                    blend_ = false;
                    return;
                }
            } else {
                auto& animation = skeleton_->animation(current_state_);
                auto length = animation.length * fps;
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
                    if (blend_next_) {
                        blend_ = true;
                        blend_next_ = false;
                    }
                    return;
                }
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
        next_keyframes_.resize(bones.size());
    }

    glm::vec3 animation_component::interpolate_translation_keyframe(const track& track, frame& current_frame, frame& next_frame, float fps) {
        if (track.tkeys.size() != 1) {
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
        } else {
            return track.tkeys[current_frame.tkey].translate;
        }
    }

    glm::quat animation_component::interpolate_quaternion_keyframe(const track& track, frame& current_frame, frame& next_frame, float fps) {
        if (track.qkeys.size() != 1) {
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
        } else {
            return track.qkeys[current_frame.qkey].rotate;
        }
    }

    void animation_component::register_at_script_engine(game& game) {
        auto& scripting_system = game.scripting_system();

        scripting_system.register_type<animation_component>("animation_component");

        scripting_system.register_member_function("animation_component", "void play_ani(const string& in)",
            +[](animation_component& component, const std::string& state) { component.change_state(state); });
        scripting_system.register_member_function("animation_component", "void blend_ani(const string& in)",
            +[](animation_component& component, const std::string& state) { component.change_state_blend(state); });
        scripting_system.register_member_function("animation_component", "bool is_playing(const string& in)",
            +[](animation_component& component, const std::string& state) { return component.is_playing(state); });
        scripting_system.register_member_function("animation_component", "bool is_blending()",
            +[](animation_component& component) { return component.is_blending(); });

        scripting_system.register_member_function("entity_impl",
            "animation_component& add_animation_component(const string& in mesh, const string& in skeleton)",
            +[](entity& owner, const std::string& mesh, const std::string& skeleton) -> animation_component& {
                return owner.emplace<animation_component>(mesh, skeleton);
            });
        scripting_system.register_member_function("entity_impl", "animation_component@ get_animation_component()",
            +[](entity& owner) { return owner.component<animation_component>(); });
    }

    animation_component::animation_component(game& game, entity& owner)
    : reflective(game, owner), current_state_{""}, next_state_{""}, elapsed_time_{0.f}, blend_{false}, blend_next_{false} {
        game_.rendering_system().register_component(this);
        game_.animation_system().register_component(this);
    }

    void animation_component::register_reflection() {
        register_property<std::string>("mesh", nullptr, &animation_component::load);
        register_property<std::string>("skeleton", nullptr, &animation_component::load_skeleton);
    }
}
