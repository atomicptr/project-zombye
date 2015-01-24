#include <iostream>
#include <stdexcept>

#include <animation_converter/animation_converter.hpp>

#include <glm/gtx/string_cast.hpp>

void to_string(const glm::quat& q) {
    std::cout << "fqat(" << glm::angle(q) << ", " << glm::axis(q).x 
        << ", " << glm::axis(q).y << ", " << glm::axis(q).z << ")" << std::endl;
}

namespace devtools {
    animation_converter::animation_converter(const std::string& in, const std::string& out) {
        is_.LoadFile(in.c_str());
        if (is_.Error()) {
            throw std::runtime_error{"could not open input file" + in};
        }
        os_.open(out, std::ios::trunc | std::ios::binary);
        if (!os_.is_open()) {
            throw std::runtime_error{"could not open output file " + out};
        }
    }

    animation_converter::~animation_converter() noexcept {
        if (os_.is_open()) {
            os_.close();
        }
    }

    void animation_converter::parse(const std::string& name) {
        auto skeleton = is_.FirstChildElement("skeleton");
        if (!skeleton) {
            throw std::runtime_error{"no skeleton defined"};
        }
        auto bones = skeleton->FirstChildElement("bones");

        auto bone = bones->FirstChildElement("bone");
        while (bone) {
            auto b = devtools::bone{};
            b.id = bone->UnsignedAttribute("id");
            b.parent = -1;

            auto name = bone->Attribute("name");

            auto position = bone->FirstChildElement("position");
            auto x = position->FloatAttribute("x");
            auto y = position->FloatAttribute("y");
            auto z = position->FloatAttribute("z");

            auto rotation = bone->FirstChildElement("rotation");
            auto angle = rotation->FloatAttribute("angle");
            auto axis = rotation->FirstChildElement("axis");
            auto ax = axis->FloatAttribute("x");
            auto ay = axis->FloatAttribute("y");
            auto az = axis->FloatAttribute("z");
            auto axis_ = glm::vec3{ax, ay, az};
            auto rot = glm::angleAxis(angle, axis_);

            auto norm = glm::normalize(rot);
            auto transform = glm::toMat4(norm);
            transform[3].x = x;
            transform[3].y = y;
            transform[3].z = z;

            b.transform = transform;

            bone_hierachy_.insert(std::make_pair(name, b));

            bone = bone->NextSiblingElement();
        }

        auto bonehierachy = bones->NextSiblingElement();
        auto boneparent = bonehierachy->FirstChildElement("boneparent");

        while (boneparent) {
            auto parent = boneparent->Attribute("parent");
            auto bone = boneparent->Attribute("bone");
            bone_hierachy_[bone].parent = bone_hierachy_[parent].id;
            bone_hierachy_[bone].transform = bone_hierachy_[parent].transform * bone_hierachy_[bone].transform;

            boneparent = boneparent->NextSiblingElement();
        }

        for (auto& b : bone_hierachy_) {
            b.second.transform = glm::inverse(b.second.transform);
            bones_.emplace_back(b.second);
        }

        std::sort(bones_.begin(), bones_.end(), [](auto& b1, auto& b2){ return b1.id < b2.id; });

        auto animations = bonehierachy->NextSiblingElement();
        auto animation = animations->FirstChildElement("animation");
        while (animation) {
            auto anim = devtools::animation{};

            anim.name = animation->Attribute("name");
            anim.length = animation->FloatAttribute("length");

            auto tracks = animation->FirstChildElement("tracks");
            auto track = tracks->FirstChildElement("track");
            while (track) {
                auto t = devtools::track{};
                auto bone = track->Attribute("bone");

                t.id = bone_hierachy_[bone].id;
                t.parent = bone_hierachy_[bone].parent;

                auto keyframes = track->FirstChildElement("keyframes");
                auto keyframe = keyframes->FirstChildElement("keyframe");
                while (keyframe) {
                    auto k = devtools::keyframe{};

                    k.time = keyframe->FloatAttribute("time");

                    auto translate = keyframe->FirstChildElement("translate");
                    auto x = translate->FloatAttribute("x");
                    auto y = translate->FloatAttribute("y");
                    auto z = translate->FloatAttribute("z");
                    k.translate = glm::vec3{x, y, z};

                    auto rotate = keyframe->FirstChildElement("rotate");
                    auto angle = rotate->FloatAttribute("angle");
                    auto axis = rotate->FirstChildElement("axis");
                    auto ax = axis->FloatAttribute("x");
                    auto ay = axis->FloatAttribute("y");
                    auto az = axis->FloatAttribute("z");
                    k.rotate = glm::normalize(glm::angleAxis(angle, glm::vec3{ax, ay, az}));

                    auto scale = keyframe->FirstChildElement("scale");
                    auto sx = scale->FloatAttribute("x");
                    auto sy = scale->FloatAttribute("y");
                    auto sz = scale->FloatAttribute("z");
                    k.scale = glm::vec3{sx, sy, sz};

                    t.keyframes.emplace_back(k);

                    keyframe = keyframe->NextSiblingElement();
                }

                anim.tracks.insert(std::make_pair(t.id, t));
                t.parent = bones_[t.parent].id;

                track = track->NextSiblingElement();
            }

            animations_.emplace_back(anim);

            animation = animation->NextSiblingElement();
        }
    }

    void animation_converter::serialize() {
        auto size = bones_.size();
        os_.write(reinterpret_cast<char*>(&size), sizeof(size_t));
        for (auto& b : bones_) {
            os_.write(reinterpret_cast<char*>(&b), sizeof(bone));
        }

        size = animations_.size();
        os_.write(reinterpret_cast<char*>(&size), sizeof(size_t));
        for (auto& a : animations_) {
            size = a.name.length();
            os_.write(reinterpret_cast<char*>(&size), sizeof(size_t));
            os_.write(a.name.c_str(), size);

            os_.write(reinterpret_cast<char*>(&a.length), sizeof(float));

            size = a.tracks.size();
            os_.write(reinterpret_cast<char*>(&size), sizeof(size_t));
            for (auto& t : a.tracks) {
                os_.write(reinterpret_cast<char*>(&t.second.id), sizeof(int));
                os_.write(reinterpret_cast<char*>(&t.second.parent), sizeof(int));

                size = t.second.keyframes.size();
                os_.write(reinterpret_cast<char*>(&size), sizeof(size_t));
                for (auto& k : t.second.keyframes) {
                    os_.write(reinterpret_cast<char*>(&k), sizeof(keyframe));
                }
            }
        }
    }
}