#include <iostream>

#include <animation_converter/animation_converter.hpp>

namespace devtools {
    animation_converter::animation_converter(const std::string& in, const std::string& out) {
        is_.open(in);
        if (!is_.is_open()) {
            throw std::runtime_error{"could not open input file " + in};
        }
        os_.open(out, std::ios::trunc | std::ios::binary);
        if (!os_.is_open()) {
            throw std::runtime_error{"could not open output file " + out};
        }
    }

    animation_converter::~animation_converter() noexcept {
        if (is_.is_open()) {
            is_.close();
        }
        if (os_.is_open()) {
            os_.close();
        }
    }

    void animation_converter::parse() {
        auto version = std::string{};
        std::getline(is_, version);
        if (version != "version 1") {
            throw std::invalid_argument{"file format is not supported"};
        }
        auto line = std::string{};
        auto dump = std::string{};
        std::getline(is_, line);
        if (line != "nodes") {
            throw std::invalid_argument("no rig defined");
        }
        while (is_.peek() != 'e') {
            bone b;
            is_ >> b.id;
            is_ >> dump;
            is_ >> b.parent;
            bones_.push_back(b);
            std::getline(is_, line);
        }
        std::getline(is_, line);
        std::getline(is_, line);
        if (line != "skeleton") {
            throw std::invalid_argument("no animation data provided");
        }
        std::getline(is_, line);
        while (line != "end") {
            if (line.substr(0, 4) == "time") {
                frames_.emplace_back(frame{});
            }
            bone_pose bp;
            is_ >> bp.id;
            is_ >> bp.loc_trans[0];
            is_ >> bp.loc_trans[1];
            is_ >> bp.loc_trans[2];
            float rx;
            float ry;
            float rz;
            is_ >> rx;
            is_ >> ry;
            is_ >> rz;
            float alpha = rx;
            float x = 1.f;
            float y = ry / alpha;
            float z = rz / alpha;
            auto rot = glm::quat{alpha, glm::vec3{x, y , z}};
            rot = glm::normalize(rot);
            bp.loc_rot = rot;
            frames_.back().emplace_back(bp);
            std::getline(is_, line);
            if (is_.peek() == 't' || is_.peek() == 'e') {
                std::getline(is_, line);
            }
        }
        if (frames_.size() != 1) {
            return;
        }
        auto triangles = std::string{};
        while (triangles != "triangles") {
            std::getline(is_, triangles);
        }
        std::getline(is_, line);
        auto material = line;
        submeshes_.insert(std::make_pair(material, std::vector<vertex>{}));
        auto i = int{0};
        while (line != "end") {
            auto v = vertex{};
            is_ >> v.parent_bone;
            is_ >> v.pos[0];
            is_ >> v.pos[1];
            is_ >> v.pos[2];
            is_ >> v.nor[0];
            is_ >> v.nor[1];
            is_ >> v.nor[2];
            is_ >> v.tex[0];
            is_ >> v.tex[1];
            is_ >> v.link_count;
            if (v.link_count > 4) {
                throw std::invalid_argument("vertex affected by more than four bones");
            }
            for (auto i = 0; i < 4; ++i) {
                if (i < v.link_count) {
                    is_ >> v.links[i].bone;
                    is_ >> v.links[i].weight;
                }
                v.links[i].bone = -1;
                v.links[i].weight = 0.f;
            }
            submeshes_[material].push_back(v);
            std::getline(is_, line);
            ++i;
            if (i > 2) {
                std::getline(is_, line);
                if (line != "end") {
                    material = line;
                    if (submeshes_.find(material) == submeshes_.end()) {
                        submeshes_.insert(std::make_pair(material, std::vector<vertex>{}));
                    }
                    i = 0;
                }
            }
        }
    }

    void animation_converter::serialize() {
        if (frames_.size() != 1) {
            auto size = frames_.size();
            os_.write(reinterpret_cast<char*>(&size), sizeof(size_t));
            for (auto& f : frames_) {
                size = f.size();
                os_.write(reinterpret_cast<char*>(&size), sizeof(size_t));
                for (auto& b : f) {
                    int id = b.id;
                    os_.write(reinterpret_cast<char*>(&id), sizeof(int));
                    auto value = reinterpret_cast<char*>(&b.loc_trans[0]);
                    os_.write(value,  sizeof(float));
                    value = reinterpret_cast<char*>(&b.loc_trans[1]);
                    os_.write(value,  sizeof(float));
                    value = reinterpret_cast<char*>(&b.loc_trans[2]);
                    os_.write(value,  sizeof(float));
                    value = reinterpret_cast<char*>(&b.loc_rot[0]);
                    os_.write(value,  sizeof(float));
                    value = reinterpret_cast<char*>(&b.loc_rot[1]);
                    os_.write(value,  sizeof(float));
                    value = reinterpret_cast<char*>(&b.loc_rot[2]);
                    os_.write(value,  sizeof(float));
                    value = reinterpret_cast<char*>(&b.loc_rot[3]);
                    os_.write(value,  sizeof(float));
                }
            }
            return;
        }
        auto bone_count = bones_.size();
        os_.write(reinterpret_cast<char*>(&bone_count), sizeof(size_t));
        for (auto& b : bones_) {
            os_.write(reinterpret_cast<char*>(&b.id), sizeof(int));
            os_.write(reinterpret_cast<char*>(&b.parent), sizeof(int));
        }
        auto vec_size = size_t{0};
        for (auto& m : submeshes_) {
            vec_size += m.second.size();
        }
        os_.write(reinterpret_cast<char*>(&vec_size), sizeof(size_t));
        for (auto& s : submeshes_) {
            for (auto& v : s.second) {
                auto size = sizeof(float);
                auto value = reinterpret_cast<char*>(&v.pos[0]);
                os_.write(value, size);
                value = reinterpret_cast<char*>(&v.pos[1]);
                os_.write(value, size);
                value = reinterpret_cast<char*>(&v.pos[2]);
                os_.write(value, size);
                value = reinterpret_cast<char*>(&v.nor[0]);
                os_.write(value, size);
                value = reinterpret_cast<char*>(&v.nor[1]);
                os_.write(value, size);
                value = reinterpret_cast<char*>(&v.nor[2]);
                os_.write(value, size);
                value = reinterpret_cast<char*>(&v.tex[0]);
                os_.write(value, size);
                value = reinterpret_cast<char*>(&v.tex[1]);
                os_.write(value, size);
                value = reinterpret_cast<char*>(&v.parent_bone);
                os_.write(value, sizeof(int));
                value = reinterpret_cast<char*>(&v.link_count);
                os_.write(value, sizeof(int));
                for (auto i = 0; i < 4; ++i) {
                    value = reinterpret_cast<char*>(&v.links[i].bone);
                    os_.write(value, sizeof(int));
                    value = reinterpret_cast<char*>(&v.links[i].weight);
                    os_.write(value, size);
                }
            }
        }
        vec_size = submeshes_.size();
        os_.write(reinterpret_cast<char*>(&vec_size), sizeof(size_t));
        auto offset = size_t{0};
        for (auto& s : submeshes_) {
            os_.write(reinterpret_cast<char*>(&offset), sizeof(size_t));
            auto vertex_count = s.second.size();
            os_.write(reinterpret_cast<char*>(&vertex_count), sizeof(size_t));
            offset = vertex_count;
        }
        vec_size = submeshes_.size();
        os_.write(reinterpret_cast<char*>(&vec_size), sizeof(size_t));
        for (auto& s : submeshes_) {
            auto str_size = s.first.length();
            os_.write(reinterpret_cast<char*>(&str_size), sizeof(size_t));
            os_.write(s.first.c_str(), str_size);
        }
    }
}