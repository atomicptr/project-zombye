#include <stdexcept>
#include <iostream>

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
        if (scene_->HasAnimations()) {
            // TODO: animation baking
        } else {
            for (auto i = 0; i < scene_->mNumMeshes; ++i) {
                auto mesh = scene_->mMeshes[i];
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
                auto material = scene_->mMaterials[mesh->mMaterialIndex];
                aiString ai_name;
                material->Get(AI_MATKEY_NAME, ai_name);
                auto mangled_name = std::string{ai_name.C_Str()};
                auto name = mangled_name.substr(0, mangled_name.size() - 9);
                submeshes_.insert(std::make_pair(name, std::vector<index>{}));
                for (auto j = 0; j < mesh->mNumFaces; ++j) {
                    auto face = mesh->mFaces[j];
                    for (auto l = 0; l < face.mNumIndices; ++l) {
                        submeshes_[name].emplace_back(face.mIndices[l]);
                    }
                }
            }
        }
    }

    void mesh_converter::serialize() {
        auto vec_size = vertices_.size();
        os_.write(reinterpret_cast<char*>(&vec_size), sizeof(size_t));
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
        auto index_count = size_t{0};
        for (auto& s : submeshes_) {
            index_count += s.second.size();
        }
        os_.write(reinterpret_cast<char*>(&index_count), sizeof(size_t));
        for (auto& s : submeshes_) {
            for (auto i : s.second) {
                os_.write(reinterpret_cast<char*>(&i), sizeof(unsigned int));
            }
        }
        vec_size = submeshes_.size();
        os_.write(reinterpret_cast<char*>(&vec_size), sizeof(size_t));
        auto offset = size_t{0};
        for (auto& s : submeshes_) {
            os_.write(reinterpret_cast<char*>(&offset), sizeof(size_t));
            index_count = s.second.size();
            os_.write(reinterpret_cast<char*>(&index_count), sizeof(size_t));
            offset = index_count;
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