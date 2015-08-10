#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <glm/gtx/string_cast.hpp>

#include <mesh_converter/mesh_converter.hpp>

namespace devtools {
    mesh_converter::mesh_converter(const std::string& input_file, const std::string& output_path) {
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

    void mesh_converter::run(bool collision_meshes) {
        std::ifstream material_database("material_database.json");
        if (!material_database.is_open()) {
            std::ofstream mdb("material_database.json", std::ios::trunc);
            if (!mdb.is_open()) {
                throw std::runtime_error("could not open material database");
            }
            Json::Value root;
            root["id_count"] = Json::Value{0};
            Json::StyledWriter sw;
            auto output_string = sw.write(root);
            mdb << output_string;
            mdb.close();
            material_database.open("material_database.json");
            if (!material_database.is_open()) {
                throw std::runtime_error("could not open material database");
            }
        }
        Json::Value mdb_root;
        Json::Reader mdb_reader;
        if (!mdb_reader.parse(material_database, mdb_root)) {
            throw std::runtime_error("could not parse material database\n" + mdb_reader.getFormattedErrorMessages());
        }
        material_database.close();

        auto id_count = mdb_root["id_count"].asUInt();

        for (auto it = root_.begin(); it != root_.end(); ++it) {
            std::string mesh_name = it.key().asString();
            auto& value = *it;

            std::vector<vertex> vertices;
            std::vector<skin> skin_attributes;
            auto vertices_data = value["vertices"];
            if (vertices_data.isNull()) {
                throw std::runtime_error("no vertex data in " + mesh_name);
            }
            for (auto& v : vertices_data) {
                auto position = v["position"];
                if (position.isNull()) {
                    throw std::runtime_error("no position attribute in vertex of " + mesh_name);
                }
                if (position.size() != 3) {
                    throw std::runtime_error("position attribute in " + mesh_name + " requires 3 elements");
                }
                auto pos = glm::vec3{position[0].asFloat(), position[2].asFloat(), -position[1].asFloat()};

                auto texcoord = v["texcoord"];
                if (texcoord.isNull()) {
                    throw std::runtime_error("no texcoord attribute in vertex of " + mesh_name);
                }
                if (texcoord.size() != 2) {
                    throw std::runtime_error("texcoord attribute in " + mesh_name + " requires 2 elements");
                }
                auto tex = glm::vec2{texcoord[0].asFloat(), texcoord[1].asFloat()};

                auto normal = v["normal"];
                if (normal.isNull()) {
                    throw std::runtime_error("no normal attribute in vertex of " + mesh_name);
                }
                if (normal.size() != 3) {
                    throw std::runtime_error("normal attribute in " + mesh_name + " requires 3 elements");
                }
                auto nor = glm::vec3{normal[0].asFloat(), normal[2].asFloat(), -normal[1].asFloat()};

                auto indices = v["indices"];
                auto ind = glm::ivec4{0};
                if (!indices.isNull()) {
                    if (indices.size() == 0) {
                        throw std::runtime_error("indices attribute in " + mesh_name + " requires at least 1 element");
                    }
                    ind = glm::ivec4{indices[0].asUInt(), indices[1].asUInt(), indices[2].asUInt(), indices[3].asUInt()};
                }

                auto weights = v["weights"];
                auto wei = glm::vec4{0.f};
                if (!weights.isNull()) {
                    if (weights.size() == 0) {
                        throw std::runtime_error("weights attribute in " + mesh_name + " requires at least 1 element");
                    }
                    wei = glm::vec4{weights[0].asFloat(), weights[1].asFloat(), weights[2].asFloat(), weights[3].asFloat()};
                }

                vertex vert;
                vert.position = pos;
                vert.texcoord = tex;
                vert.normal = nor;
                vertices.emplace_back(vert);

                if (!indices.isNull() && !weights.isNull()) {
                    skin sk;
                    sk.indices = ind;
                    sk.weights = wei;
                    skin_attributes.emplace_back(sk);
                }
            }

            std::vector<unsigned int> indices;
            std::vector<submesh> submeshes;
            auto submesh_data = value["submeshes"];
            if (submesh_data.isNull()) {
                throw std::runtime_error("no submesh data in " + mesh_name);
            }
            for (auto& s : submesh_data) {
                submesh sm;
                sm.offset = indices.size();

                auto indice_data = s["indices"];
                if (indice_data.isNull()) {
                    throw std::runtime_error("no index data in submesh of " + mesh_name);
                }
                for (auto& t : indice_data) {
                    if (t.size() != 3) {
                        throw std::runtime_error("sizeof indices per triangle must be 3");
                    }
                    auto index0 = t[0].asUInt();
                    auto index1 = t[1].asUInt();
                    auto index2 = t[2].asUInt();

                    indices.emplace_back(index0);
                    indices.emplace_back(index1);
                    indices.emplace_back(index2);

                    auto& v0 = vertices[index0];
                    auto& v1 = vertices[index1];
                    auto& v2 = vertices[index2];

                    auto e1 = v1.position - v0.position;
                    auto e2 = v2.position - v0.position;

                    auto delta_u1 = v1.texcoord.x - v0.texcoord.x;
                    auto delta_v1 = v1.texcoord.y - v0.texcoord.y;
                    auto delta_u2 = v2.texcoord.x - v0.texcoord.x;
                    auto delta_v2 = v2.texcoord.y - v0.texcoord.y;

                    auto f = 1.f / (delta_u1 * delta_v2 - delta_u2 * delta_v1);

                    auto tangent = f * delta_v2 * e1 - delta_v1 * e2;

                    v0.tangent += tangent;
                    v1.tangent += tangent;
                    v2.tangent += tangent;
                }

                sm.index_count = indices.size() - sm.offset;

                auto textures = s["textures"];
                if (textures.isNull()) {
                    throw std::runtime_error("no texture data in submesh of " + mesh_name);
                }

                auto diffuse_texture = textures["diffuse"];
                if (diffuse_texture.isNull()) {
                    throw std::runtime_error("no diffuse texture in texture data in submesh of " + mesh_name);
                }
                auto texture_path = diffuse_texture.asString();
                auto texture_name = std::string{texture_path.begin() + texture_path.rfind("/") + 1, texture_path.end()};

                std::ifstream tex_in(texture_path, std::ios::binary);
                if (!tex_in.is_open()) {
                    throw std::runtime_error("could not open texture to read " + texture_path);
                }
                auto texture_id = mdb_root[texture_name];
                if (texture_id.isNull()) {
                    ++id_count;
                    texture_id = mdb_root[texture_name] = Json::Value{id_count};
                    mdb_root["id_count"] = Json::Value{id_count};
                }
                auto texture_output_path = output_path_ + "texture/" + std::to_string(texture_id.asUInt()) + ".dds";
                std::ofstream tex_out(texture_output_path, std::ios::binary | std::ios::trunc);
                if (!tex_out.is_open()) {
                    throw std::runtime_error("could not open texture to write " + texture_output_path);
                }

                tex_out << tex_in.rdbuf();
                tex_in.close();
                tex_out.close();

                sm.diffuse = texture_id.asUInt();

                auto normal_texture = textures["normal"];
                if (normal_texture.isNull()) {
                    throw std::runtime_error("no normal texture in texture data in submesh of " + mesh_name);
                }
                texture_path = normal_texture.asString();
                texture_name = std::string{texture_path.begin() + texture_path.rfind("/") + 1, texture_path.end()};

                tex_in.open(texture_path, std::ios::binary);
                if (!tex_in.is_open()) {
                    throw std::runtime_error("could not open texture to read " + texture_path);
                }
                texture_id = mdb_root[texture_name];
                if (texture_id.isNull()) {
                    ++id_count;
                    texture_id = mdb_root[texture_name] = Json::Value{id_count};
                    mdb_root["id_count"] = Json::Value{id_count};
                }
                texture_output_path = output_path_ + "texture/" + std::to_string(texture_id.asUInt()) + ".dds";
                tex_out.open(texture_output_path, std::ios::binary | std::ios::trunc);
                if (!tex_out.is_open()) {
                    throw std::runtime_error("could not open texture to write " + texture_output_path);
                }

                tex_out << tex_in.rdbuf();
                tex_in.close();
                tex_out.close();

                sm.normal = texture_id.asUInt();

                auto material_texture = textures["material"];
                if (material_texture.isNull()) {
                    throw std::runtime_error("no material texture in texture data in submesh of " + mesh_name);
                }
                texture_path = material_texture.asString();
                texture_name = std::string{texture_path.begin() + texture_path.rfind("/") + 1, texture_path.end()};

                tex_in.open(texture_path, std::ios::binary);
                if (!tex_in.is_open()) {
                    throw std::runtime_error("could not open texture to read " + texture_path);
                }
                texture_id = mdb_root[texture_name];
                if (texture_id.isNull()) {
                    ++id_count;
                    texture_id = mdb_root[texture_name] = Json::Value{id_count};
                    mdb_root["id_count"] = Json::Value{id_count};
                }
                texture_output_path = output_path_ + "texture/" + std::to_string(texture_id.asUInt()) + ".dds";
                tex_out.open(texture_output_path, std::ios::binary | std::ios::trunc);
                if (!tex_out.is_open()) {
                    throw std::runtime_error("could not open texture to write " + texture_output_path);
                }

                tex_out << tex_in.rdbuf();
                tex_in.close();
                tex_out.close();

                sm.material = texture_id.asUInt();

                submeshes.emplace_back(sm);
            }

            auto parallax_mapping = value["parallax"];
            if (parallax_mapping.isNull()) {
                throw std::runtime_error("no property 'parallax' in " + mesh_name);
            }

            std::ofstream mdb_write("material_database.json", std::ios::trunc);
            if (!mdb_write.is_open()) {
                throw std::runtime_error("could not open material database to write");
            }
            Json::StyledWriter sw;
            mdb_write << sw.write(mdb_root);
            mdb_write.close();

            header h;
            h.vertex_count = vertices.size();
            h.index_count = indices.size();
            h.submesh_count = submeshes.size();
            h.parallax_mapping = parallax_mapping.asBool();

            std::string output_file = output_path_ + "meshes/" + mesh_name + ".msh";
            std::ofstream output(output_file, std::ios::binary | std::ios::trunc);
            if (!output.is_open()) {
                throw std::runtime_error("could not open output file " + output_file);
            }

            output.write(reinterpret_cast<char*>(&h), sizeof(header));
            for (auto i = 0ul; i < vertices.size(); ++i) {
                output.write(reinterpret_cast<char*>(&vertices[i]), sizeof(vertex));
                if (skin_attributes.size() > 0) {
                    output.write(reinterpret_cast<char*>(&skin_attributes[i]), sizeof(skin));
                }
            }
            output.write(reinterpret_cast<char*>(indices.data()), indices.size() * sizeof(unsigned int));
            output.write(reinterpret_cast<char*>(submeshes.data()), submeshes.size() * sizeof(submesh));

            output.close();

            if (collision_meshes) {
                collision_header ch;
                ch.vertex_count = vertices.size();
                ch.index_count = indices.size();

                output_file = output_path_ + "meshes/" + mesh_name + ".col";
                output.open(output_file, std::ios::binary | std::ios::trunc);
                if (!output.is_open()) {
                    throw std::runtime_error("could not open output file " + output_file);
                }

                output.write(reinterpret_cast<char*>(&ch), sizeof(collision_header));
                for (auto i = 0ul; i < vertices.size(); ++i) {
                    output.write(reinterpret_cast<char*>(&vertices[i].position), sizeof(glm::vec3));
                }
                output.write(reinterpret_cast<char*>(indices.data()), indices.size() * sizeof(unsigned int));

                output.close();
            }
        }
    }
}
