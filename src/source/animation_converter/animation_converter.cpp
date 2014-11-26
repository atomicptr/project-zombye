#include <iostream>
#include <stdexcept>

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <animation_converter/animation_converter.hpp>

namespace devtools {
    animation_converter::animation_converter(const std::string& in, const std::string& out) {
        scene_ = is_.ReadFile(in,
            aiProcess_CalcTangentSpace |
            aiProcess_JoinIdenticalVertices |
            aiProcess_Triangulate);
        if (!scene_) {
            throw std::runtime_error{"could not open input file " + in};
        }
        os_.open(out, std::ios::trunc | std::ios::binary);
        if (!os_.is_open()) {
            throw std::runtime_error{"could not open output file " + out};
        }
    }

    animation_converter::~animation_converter() noexcept {
        if (scene_) {
            is_.FreeScene();
        }
        if (os_.is_open()) {
            os_.close();
        }
    }

    void animation_converter::parse(const std::string& name) {
        if (scene_->HasMeshes()) {
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

                if (mesh->HasBones()) {
                    for (auto j = 0; j < mesh->mNumBones; ++j) {
                        auto ai_bone = mesh->mBones[j];
                        bone b;
                        b.id = bone_ids_.size();
                        b.transformation = *reinterpret_cast<glm::mat4*>(&ai_bone->mOffsetMatrix);
                        bones_.emplace_back(b);
                        std::string bone_name{ai_bone->mName.C_Str()};
                        bone_ids_.insert(std::make_pair(bone_name, bone_ids_.size()));

                        for (auto k = 0; k < ai_bone->mNumWeights; ++k) {
                            auto& weight = ai_bone->mWeights[k];
                            auto& v = vertices_[submesh.base_vertex + weight.mVertexId];
                            if (v.link_count == 4) {
                                throw std::runtime_error("too many bones affecting vertex");
                            }
                            v.index[v.link_count] = b.id;
                            v.weight[v.link_count] = weight.mWeight;
                            ++v.link_count;
                        }
                    }
                } else {
                    throw std::runtime_error("model has no rig");
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
        } else if (scene_->HasAnimations()) {
            auto ai_anim = scene_->mAnimations[0];
            animation_.name = name;
            animation_.duration = ai_anim->mDuration;
            animation_.tps = ai_anim->mTicksPerSecond;

            for (auto j = 0; j < ai_anim->mNumChannels; ++j) {
                auto ai_node = ai_anim->mChannels[j];
                auto node_name = std::string{ai_node->mNodeName.C_Str()};
                node anim_node;
                anim_node.bone = bone_ids_[node_name];

                for (auto k = 0; k < ai_node->mNumPositionKeys; ++k) {
                    auto ai_key = ai_node->mPositionKeys[k];
                    vector_key key;
                    key.time = ai_key.mTime;
                    key.value = *reinterpret_cast<glm::vec3*>(&ai_key.mValue);
                    anim_node.position_keys.emplace_back(key);
                }

                for (auto k = 0; k < ai_node->mNumRotationKeys; ++k) {
                    auto ai_key = ai_node->mRotationKeys[k];
                    quaternion_key key;
                    key.time = ai_key.mTime;
                    key.value = *reinterpret_cast<glm::quat*>(&ai_key.mValue);
                    anim_node.rotation_keys.emplace_back(key);
                }

                for (auto k = 0; k < ai_node->mNumScalingKeys; ++k) {
                    auto ai_key = ai_node->mScalingKeys[k];
                    vector_key key;
                    key.time = ai_key.mTime;
                    key.value = *reinterpret_cast<glm::vec3*>(&ai_key.mValue);
                    anim_node.scaling_keys.emplace_back(key);
                }

                animation_.nodes.emplace_back(anim_node);
            }
        }
    }

    void animation_converter::serialize() {
        if (scene_->HasMeshes()) {
            auto bone_count = bones_.size();
            os_.write(reinterpret_cast<char*>(&bone_count), sizeof(size_t));
            for (auto& b : bones_) {
                os_.write(reinterpret_cast<char*>(&b), sizeof(bone));
            }

            auto vertex_count = vertices_.size();
            os_.write(reinterpret_cast<char*>(&vertex_count), sizeof(size_t));
            for (auto& v : vertices_) {
                os_.write(reinterpret_cast<char*>(&v), sizeof(vertex) - sizeof(unsigned int));
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
        } else if (scene_->HasAnimations()) {
            auto size = animation_.name.size();
            os_.write(reinterpret_cast<char*>(&size), sizeof(size_t));
            os_.write(animation_.name.c_str(), animation_.name.size());
            os_.write(reinterpret_cast<char*>(&animation_.duration), sizeof(float));
            os_.write(reinterpret_cast<char*>(&animation_.tps), sizeof(float));
            for (auto& n : animation_.nodes) {
                os_.write(reinterpret_cast<char*>(&n.bone), sizeof(float));

                size = n.position_keys.size();
                os_.write(reinterpret_cast<char*>(&size), sizeof(size_t));
                for (auto& pk : n.position_keys) {
                    os_.write(reinterpret_cast<char*>(&pk), sizeof(vector_key));
                }

                size = n.rotation_keys.size();
                os_.write(reinterpret_cast<char*>(&size), sizeof(size_t));
                for (auto& rk : n.rotation_keys) {
                    os_.write(reinterpret_cast<char*>(&rk), sizeof(quaternion_key));
                }

                size = n.scaling_keys.size();
                os_.write(reinterpret_cast<char*>(&size), sizeof(size_t));
                for (auto& sk : n.scaling_keys) {
                    os_.write(reinterpret_cast<char*>(&sk), sizeof(vector_key));
                }
            }
        }
    }
}