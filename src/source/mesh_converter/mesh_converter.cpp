#include <iostream>
#include <stdexcept>
#include <sstream>

#include <glm/gtx/string_cast.hpp>

#include <mesh_converter/mesh_converter.hpp>

namespace devtools {
    mesh_converter::mesh_converter(const std::string& in, const std::string& out) {
        is_.LoadFile(in.c_str());
        if (is_.Error()) {
            throw std::runtime_error{"could not open input file" + in};
        }
        os_.open(out, std::ios::trunc | std::ios::binary);
        if (!os_.is_open()) {
            throw std::runtime_error{"could not open output file " + out};
        }
    }

    mesh_converter::~mesh_converter() noexcept {
        if (os_.is_open()) {
            os_.close();
        }
    }

    void mesh_converter::parse(bool collison_geometry) {
        auto mesh = is_.FirstChildElement("mesh");
        if (!mesh) {
            throw std::runtime_error{"no mesh defined"};
        }
        auto sharedgeometry = mesh->FirstChildElement("sharedgeometry");

        auto vertexcount = sharedgeometry->UnsignedAttribute("vertexcount");
        auto vertexbuffer = sharedgeometry->FirstChildElement("vertexbuffer");
        auto vertex = vertexbuffer->FirstChildElement("vertex");
        for (auto i = 0; i < vertexcount; ++i) {
            auto pos = vertex->FirstChildElement("position");
            auto nor = vertex->FirstChildElement("normal");
            auto tex = vertex->FirstChildElement("texcoord");
            devtools::vertex v;
            v.pos.x = pos->FloatAttribute("x");
            v.pos.y = pos->FloatAttribute("y");
            v.pos.z = pos->FloatAttribute("z");
            v.nor.x = nor->FloatAttribute("x");
            v.nor.y = nor->FloatAttribute("y");
            v.nor.z = nor->FloatAttribute("z");
            v.tex.x = tex->FloatAttribute("u");
            v.tex.y = tex->FloatAttribute("v");
            vertices_.emplace_back(v);
            vertex = vertex->NextSiblingElement();
        }

        auto submeshes = sharedgeometry->NextSiblingElement("submeshes");
        auto submesh = submeshes->FirstChildElement("submesh");
        while (submesh) {
            auto material = submesh->Attribute("material");
            materials_.emplace_back(material);
            auto faces = submesh->FirstChildElement("faces");
            auto count = faces->UnsignedAttribute("count");
            devtools::submesh s;
            s.index_count = 3 * count;
            s.offset = indices_.size();
            submeshes_.emplace_back(s);
            auto face = faces->FirstChildElement("face");
            for (auto i = 0; i < count; ++i) {
                indices_.emplace_back(face->UnsignedAttribute("v1"));
                indices_.emplace_back(face->UnsignedAttribute("v2"));
                indices_.emplace_back(face->UnsignedAttribute("v3"));
                face = face->NextSiblingElement();
            }
            submesh = submesh->NextSiblingElement("submesh");
        }
    }

    void mesh_converter::serialize(bool collison_geometry) {
        auto vertex_count = vertices_.size();

        os_.write(reinterpret_cast<char*>(&vertex_count), sizeof(size_t));
        for (auto& v : vertices_) {
            os_.write(reinterpret_cast<char*>(&v), sizeof(vertex));
        }

        auto index_count = indices_.size();
        os_.write(reinterpret_cast<char*>(&index_count), sizeof(size_t));
        for (auto i : indices_) {
            os_.write(reinterpret_cast<char*>(&i), sizeof(index));
        }

        if (!collison_geometry) {
            auto submesh_count = submeshes_.size();
            os_.write(reinterpret_cast<char*>(&submesh_count), sizeof(size_t));
            for (auto& s : submeshes_) {
                os_.write(reinterpret_cast<char*>(&s), sizeof(submesh));
            }

            auto material_count = materials_.size();
            os_.write(reinterpret_cast<char*>(&material_count), sizeof(size_t));
            for (auto& m : materials_) {
                auto str_size = m.length();
                os_.write(reinterpret_cast<char*>(&str_size), sizeof(size_t));
                os_.write(m.c_str(), str_size);
            }
        }
    }
}