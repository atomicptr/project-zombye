#include <iostream>
#include <stdexcept>
#include <stack>

#include <animation_converter/animation_converter.hpp>

#include <glm/gtx/string_cast.hpp>

void to_string(const glm::quat& q) {
    std::cout << "fqat(" << glm::angle(q) << ", " << glm::axis(q).x 
        << ", " << glm::axis(q).y << ", " << glm::axis(q).z << ")" << std::endl;
}

namespace devtools {
    animation_converter::animation_converter(const std::string& input_file, const std::string& output_path) {
        output_path_ = output_path;
        if (*(output_path_.end() - 1) != '/') {
            output_path_ = "/";
        }

        std::fstream file(input_file);
        if (!file.is_open()) {
            throw std::runtime_error("could not open input file " + input_file);
        }
        if (!reader_.parse(file, root_)) {
            throw std::runtime_error("could not parse input file " + input_file + "\n" + reader_.getFormattedErrorMessages());
        }
        file.close();
    }

    void animation_converter::run() {
        for (auto it = root_.begin(); it != root_.end(); ++it) {
            std::string model_name = it.key().asString();
            auto& value = *it;

            std::unordered_map<int, bone> bone_hierachy;
            auto skeleton = value["skeleton"];
            if (skeleton.isNull()) {
                throw std::runtime_error("no skeleton data in " + model_name);
            }
            for (auto& b : skeleton) {
                bone bn;

                auto id = b["id"];
                if (id.isNull()) {
                    throw std::runtime_error("no id attribute in bone data of " + model_name);
                }
                bn.id = id.asInt();

                auto parent = b["parent"];
                if (parent.isNull()) {
                    bn.parent = -1;
                } else {
                    bn.parent = parent.asInt();
                }

                auto translation = b["translation"];
                if (translation.isNull()) {
                    throw std::runtime_error("no translation attribute in bone data of " + model_name);
                }
                if (translation.size() != 3) {
                    throw std::runtime_error("translation attribute has not the apropriate size in " + model_name);
                }
                auto trans = glm::vec3{translation[0].asFloat(), translation[1].asFloat(), translation[2].asFloat()};

                auto rotation = b["rotation"];
                if (rotation.isNull()) {
                    throw std::runtime_error("no rotation attribute in bone data of " + model_name);
                }
                if (rotation.size() != 4) {
                    throw std::runtime_error("rotation attribute has not the apropriate size in " + model_name);
                }
                auto rot = glm::quat{rotation[0].asFloat(), rotation[1].asFloat(), rotation[2].asFloat(), rotation[3].asFloat()};
                rot = glm::normalize(rot);
                if (rot.w < 0.f) {
                    rot = -rot;
                }

                auto transform = glm::toMat4(rot);
                transform[3].x = trans.x;
                transform[3].y = trans.y;
                transform[3].z = trans.z;

                bn.relative_transform = transform;
                bn.absolute_transform = transform;

                bone_hierachy.insert(std::make_pair(bn.id, bn));
            }

            /*
            for (auto i = 0; i < bone_hierachy.size(); ++i) {
                auto parent = bone_hierachy[i].parent;
                if (parent != -1) {
                    bone_hierachy[i].absolute_transform = bone_hierachy[parent].absolute_transform * bone_hierachy[i].absolute_transform;
                }
            }
            */
            std::stack<int32_t> traversal;
            traversal.push(0);
            while (!traversal.empty()) {
                auto current = traversal.top();
                traversal.pop();
                auto node = value["bone_hierachy"][std::to_string(current)];
                if (node.isNull()) {
                    continue;
                }
                for (auto& child : node) {
                    auto c = child.asInt();
                    traversal.push(c);
                    bone_hierachy[c].absolute_transform = bone_hierachy[current].absolute_transform * bone_hierachy[c].absolute_transform;
                }
            }

            std::vector<bone> bones;
            for (auto i = 0ul; i < bone_hierachy.size(); ++i) {
                bone_hierachy[i].absolute_transform = glm::inverse(bone_hierachy[i].absolute_transform);
                bones.emplace_back(bone_hierachy[i]);
            }

            auto animations = value["animations"];
            if (animations.isNull()) {
                throw std::runtime_error("no animation data in " + model_name);
            }

            header h;
            h.bone_count = bones.size();
            h.animation_count = animations.size();

            std::string output_file = output_path_ + "anims/" + model_name + ".skl";
            std::ofstream output(output_file, std::ios::binary | std::ios::trunc);
            if (!output.is_open()) {
                throw std::runtime_error("could not open output file " + output_file);
            }

            std::vector<animation> anims;
            std::vector<track> all_tracks;
            std::vector<translation_keyframe> all_tkeys;
            std::vector<rotation_keyframe> all_qkeys;
            std::vector<scale_keyframe> all_skeys;
            for (auto at = animations.begin(); at != animations.end(); ++at) {
                animation a;
                a.name = at.key().asString();

                auto& anim = *at;
                auto length = anim["length"];
                if (length.isNull()) {
                    throw std::runtime_error("animation " + a.name + " has no length attribute");
                }
                a.length = length.asFloat();

                auto tracks = anim["tracks"];
                if (tracks.isNull()) {
                    throw std::runtime_error("animation " + a.name + " has no tracks");
                }
                a.track_count = tracks.size();

                anims.emplace_back(a);

                for (auto tt = tracks.begin(); tt != tracks.end(); ++tt) {
                    auto bone_name = tt.key().asString();

                    auto& anim_track = *tt;


                    auto bone_id = anim_track["id"];
                    if (bone_id.isNull()) {
                        throw std::runtime_error("track " + bone_name + " in animation " + a.name + " has no id");
                    }
                    auto& b = bone_hierachy[bone_id.asInt()];

                    track t;
                    t.id = b.id;
                    t.parent = b.parent;

                    auto tkeys = anim_track["location"];
                    if (tkeys.isNull()) {
                        throw std::runtime_error("track " + bone_name + " in animation " + a.name + " has no translation keys");
                    }
                    t.tkey_count = tkeys.size();

                    auto qkeys = anim_track["rotation_quaternion"];
                    if (qkeys.isNull()) {
                        throw std::runtime_error("track " + bone_name + " in animation " + a.name + " has no rotation keys");
                    }
                    t.qkey_count = qkeys.size();

                    auto skeys = anim_track["scale"];
                    if (skeys.isNull()) {
                        throw std::runtime_error("track " + bone_name + " in animation " + a.name + " has no scale keys");
                    }
                    t.skey_count = skeys.size();

                    all_tracks.emplace_back(t);
                }

                for (auto tt = tracks.begin(); tt != tracks.end(); ++tt) {
                    auto bone_name = tt.key().asString();

                    auto& anim_track = *tt;

                    auto tkeys = anim_track["location"];
                    for (auto& tkey : tkeys) {
                        auto frame = tkey["frame"];
                        if (frame.isNull()) {
                            throw std::runtime_error("track " + bone_name + " in animations " + a.name + " has no frame number in keyframe");
                        }

                        auto data = tkey["data"];
                        if (data.isNull()) {
                            throw std::runtime_error("track " + bone_name + " in animation " + a.name + " has no data in keframe " +  frame.asString());
                        }
                        if (data.size() != 3) {
                            throw std::runtime_error("data in keyframe " + frame.asString() + " in track " + bone_name + " in animation " + a.name + " is too small");
                        }

                        translation_keyframe tk;
                        tk.time = frame.asFloat();
                        tk.translate = glm::vec3{data[0].asFloat(), data[2].asFloat(), -data[1].asFloat()};

                        all_tkeys.emplace_back(tk);
                    }

                    auto qkeys = anim_track["rotation_quaternion"];
                    for (auto& qkey : qkeys) {
                        auto frame = qkey["frame"];
                        if (frame.isNull()) {
                            throw std::runtime_error("track " + bone_name + " in animations " + a.name + " has no frame number in keyframe");
                        }

                        auto data = qkey["data"];
                        if (data.isNull()) {
                            throw std::runtime_error("track " + bone_name + " in animation " + a.name + " has no data in keframe " +  frame.asString());
                        }
                        if (data.size() != 4) {
                            throw std::runtime_error("data in keyframe " + frame.asString() + " in track " + bone_name + " in animation " + a.name + " is too small");
                        }

                        rotation_keyframe rk;
                        rk.time = frame.asFloat();
                        rk.rotate = glm::quat{data[0].asFloat(), data[1].asFloat(), data[2].asFloat(), data[3].asFloat()};

                        all_qkeys.emplace_back(rk);
                    }

                    auto skeys = anim_track["scale"];
                    for (auto& skey : skeys) {
                        auto frame = skey["frame"];
                        if (frame.isNull()) {
                            throw std::runtime_error("track " + bone_name + " in animations " + a.name + " has no frame number in keyframe");
                        }

                        auto data = skey["data"];
                        if (data.isNull()) {
                            throw std::runtime_error("track " + bone_name + " in animation " + a.name + " has no data in keframe " +  frame.asString());
                        }
                        if (data.size() != 3) {
                            throw std::runtime_error("data in keyframe " + frame.asString() + " in track " + bone_name + " in animation " + a.name + " is too small");
                        }

                        scale_keyframe sk;
                        sk.time = frame.asFloat();
                        sk.scale = glm::vec3{data[0].asFloat(), data[2].asFloat(), -data[1].asFloat()};

                        all_skeys.emplace_back(sk);
                    }
                }
            }

            auto animation_size = 20 + sizeof(float) + sizeof(int64_t);

            auto bone_count = int(h.bone_count);
            h.size = sizeof(header)
                + bone_count * sizeof(bone)
                + bone_count * sizeof(node) + (bone_count - 1) * sizeof(int32_t)
                + h.animation_count * animation_size
                + all_tracks.size() *  sizeof(track)
                + all_tkeys.size() * sizeof(translation_keyframe)
                + all_qkeys.size() * sizeof(rotation_keyframe)
                + all_skeys.size() * sizeof(scale_keyframe);

            output.write(reinterpret_cast<char*>(&h), sizeof(header));
            output.write(reinterpret_cast<char*>(bones.data()), bones.size() * sizeof(bone));

            auto stuff = 0ul;
            for (auto i = 0ul; i < value["bone_hierachy"].size(); ++i) {
                node n;
                n.id = i;
                auto children = value["bone_hierachy"][std::to_string(i)];
                n.children_count = children.size();
                output.write(reinterpret_cast<char*>(&n), sizeof(node));
                stuff += sizeof(node);
                for (auto& child : children) {
                    output.write(reinterpret_cast<char*>(&child), sizeof(int32_t));
                    stuff += sizeof(int32_t);
                }
            }

            auto track_ptr = 0ul;
            auto tkey_ptr = 0ul;
            auto qkey_ptr = 0ul;
            auto skey_ptr = 0ul;
            for (auto& a : anims) {
                if (a.name.size() < 20) {
                    output.write(a.name.c_str(), a.name.size() * sizeof(char));
                    for (auto i = 0ul; i < 20 - a.name.size(); ++i) {
                        output.write("\0", sizeof(char));
                    }
                } else {
                    output.write(a.name.c_str(), 20 * sizeof(char));
                }
                output.write(reinterpret_cast<char*>(&a.length), sizeof(float));
                output.write(reinterpret_cast<char*>(&a.track_count), sizeof(size_t));

                for (auto i = 0; i < a.track_count; ++i) {
                    auto& t = all_tracks[track_ptr + i];
                    output.write(reinterpret_cast<char*>(&t), sizeof(track));

                    for (auto i = 0ul; i < t.tkey_count; ++i) {
                        output.write(reinterpret_cast<char*>(&all_tkeys[tkey_ptr + i]), sizeof(translation_keyframe));
                    }
                    tkey_ptr += t.tkey_count;

                    for (auto j = 0ul; j < t.qkey_count; ++j) {
                        output.write(reinterpret_cast<char*>(&all_qkeys[qkey_ptr + j]), sizeof(rotation_keyframe));
                    }
                    qkey_ptr += t.qkey_count;

                    for (auto k = 0ul; k < t.skey_count; ++k) {
                        output.write(reinterpret_cast<char*>(&all_skeys[skey_ptr + k]), sizeof(scale_keyframe));
                    }
                    skey_ptr += t.skey_count;
                }
                track_ptr += a.track_count;
            }
        }
    }
}