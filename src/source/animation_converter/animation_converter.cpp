#include <iostream>
#include <stdexcept>

#include <animation_converter/animation_converter.hpp>

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

                auto transform = glm::toMat4(rot);
                transform[3].x = trans.x;
                transform[3].y = trans.y;
                transform[3].z = trans.z;

                bn.relative_transform = transform;
                bn.absolute_transform = transform;

                bone_hierachy.insert(std::make_pair(bn.id, bn));
            }

            for (auto i = 0; i < bone_hierachy.size(); ++i) {
                auto parent = bone_hierachy[i].parent;
                if (parent != -1) {
                    bone_hierachy[i].absolute_transform = bone_hierachy[parent].absolute_transform * bone_hierachy[i].absolute_transform;
                }
            }

            std::vector<bone> bones;
            for (auto& b : bone_hierachy) {
                b.second.absolute_transform = glm::inverse(b.second.absolute_transform);
                bones.emplace_back(b.second);
            }

            header h;
            h.bone_count = bones.size();

            std::string output_file = output_path_ + "anims/" + model_name + ".skl";
            std::ofstream output(output_file, std::ios::binary | std::ios::trunc);
            if (!output.is_open()) {
                throw std::runtime_error("could not open output file " + output_file);
            }

            output.write(reinterpret_cast<char*>(&h), sizeof(header));
            output.write(reinterpret_cast<char*>(bones.data()), bones.size() * sizeof(bone));
        }
    }
}