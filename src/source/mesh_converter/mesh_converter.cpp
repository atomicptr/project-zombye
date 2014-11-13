#include <stdexcept>
#include <iostream>

#include <mesh_converter/mesh_converter.hpp>

namespace devtools {
    mesh_converter::mesh_converter(const std::string& in, const std::string& out) {
        is_.open(in);
        if (!is_.is_open()) {
            throw std::runtime_error{"could not open input file " + in};
        }
        os_.open(out, std::ios::trunc | std::ios::binary);
        if (!os_.is_open()) {
            throw std::runtime_error{"could not open output file " + out};
        }
    }

    mesh_converter::~mesh_converter() noexcept {
        if (is_.is_open()) {
            is_.close();
        }
        if (os_.is_open()) {
            os_.close();
        }
    }

    void mesh_converter::parse() {
        auto version = std::string{};
        std::getline(is_, version);
        if (version != "version 1") {
            throw std::invalid_argument{"file format is not supported"};
        }
        auto triangles = std::string{};
        while (triangles != "triangles") {
            std::getline(is_, triangles);
        }
        auto line = std::string{};
        auto dump = std::string{};
        std::getline(is_, line);
        auto i = int{0};
        while (line != "end") {
            is_ >> dump;
            auto v = vertex{};
            is_ >> v.pos[0];
            is_ >> v.pos[1];
            is_ >> v.pos[2];
            is_ >> v.nor[0];
            is_ >> v.nor[1];
            is_ >> v.nor[2];
            is_ >> v.tex[0];
            is_ >> v.tex[1];
            vertices_.push_back(v);
            std::getline(is_, line);
            ++i;
            if (i > 2) {
                std::getline(is_, line);
                i = 0;
            }
        }
    }

    void mesh_converter::serialize() {
        auto vec_size = vertices_.size();
        auto count = reinterpret_cast<char*>(&vec_size);
        os_.write(count, sizeof(size_t));
        for (auto& v : vertices_) {
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
        }
    }
}