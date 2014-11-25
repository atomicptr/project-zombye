#include <iostream>
#include <stdexcept>

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <mesh_converter/mesh_converter.hpp>

namespace devtools {
    mesh_converter::mesh_converter(const std::string& in, const std::string& out) {
        scene_ = is_.ReadFile(in,
            aiProcess_CalcTangentSpace |
            aiProcess_JoinIdenticalVertices |
            aiProcess_Triangulate |
            aiProcess_PreTransformVertices);
        if (!scene_) {
            throw std::runtime_error{"could not open input file " + in};
        }
        os_.open(out, std::ios::trunc | std::ios::binary);
        if (!os_.is_open()) {
            throw std::runtime_error{"could not open output file " + out};
        }
    }

    mesh_converter::~mesh_converter() noexcept {
        if (scene_) {
            is_.FreeScene();
        }
        if (os_.is_open()) {
            os_.close();
        }
    }

    void mesh_converter::parse() {
        for (auto i = 0; i < scene_->mNumMeshes; ++i) {
            auto mesh = scene_->mMeshes[i];

            submesh submesh;
            submesh.index_count = mesh->mNumFaces * 3;
            submesh.offset = indices_.size();
            submesh.base_vertex = vertices_.size();
            submeshes_.emplace_back(submesh);

            for (auto k = 0; k < mesh->mNumVertices; ++k) {
                auto pos = mesh->mVertices[k];
                auto nor = mesh->mNormals[k];
                auto tex = mesh->mTextureCoords[0][k];
                vertex v;
                v.pos[0] = pos.x;
                v.pos[1] = pos.y;
                v.pos[2] = pos.z;
                v.nor[0] = nor.x;
                v.nor[1] = nor.y;
                v.nor[2] = nor.z;
                v.tex[0] = tex.x;
                v.tex[1] = tex.y;
                vertices_.emplace_back(v);
            }

            for (auto j = 0; j < mesh->mNumFaces; ++j) {
                auto face = mesh->mFaces[j];
                for (auto l = 0; l < 3; ++l) {
                    indices_.emplace_back(face.mIndices[l]);
                }
            }

            auto material = scene_->mMaterials[mesh->mMaterialIndex];
            aiString ai_name;
            material->Get(AI_MATKEY_NAME, ai_name);
            auto mangled_name = std::string{ai_name.C_Str()};
            auto name = mangled_name.substr(0, mangled_name.size() - 9);
            materials_.emplace_back(name);
        }
    }

    void mesh_converter::serialize() {
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