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

    void mesh_converter::run() {
        for (auto it = root_.begin(); it != root_.end(); ++it) {
            std::string mesh_name = it.key().asString();
            auto& value = *it;

            std::vector<vertex> vertices;
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
                auto pos = glm::vec3{position[0].asFloat(), position[1].asFloat(), position[2].asFloat()};

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
                auto nor = glm::vec3{normal[0].asFloat(), normal[1].asFloat(), normal[2].asFloat()};

                vertex vert;
                vert.position = pos;
                vert.texcoord = tex;
                vert.normal = nor;
                vertices.emplace_back(vert);
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
                    indices.emplace_back(t[0].asUInt());
                    indices.emplace_back(t[1].asUInt());
                    indices.emplace_back(t[2].asUInt());
                }

                sm.index_count = indices.size();
                submeshes.emplace_back(sm);
            }

            header h;
            h.vertex_count = vertices.size();

            std::string output_file = output_path_ + "meshes/" + mesh_name + ".msh";
            std::ofstream output(output_file, std::ios::binary | std::ios::trunc);
            if (!output.is_open()) {
                throw std::runtime_error("could not open output file " + output_file);
            }

            output.write(reinterpret_cast<char*>(&h), sizeof(header));
            output.write(reinterpret_cast<char*>(vertices.data()), vertices.size() * sizeof(vertex));
            output.write(reinterpret_cast<char*>(indices.data()), indices.size() * sizeof(unsigned int));
            output.write(reinterpret_cast<char*>(submeshes.data()), submeshes.size() * sizeof(submesh));

            output.close();
        }
    }
}