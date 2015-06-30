#include <stdexcept>

#include <mesh_converter/mesh_converter.hpp>

int main(int argc, char const* argv[]) {
    if (argc < 3) {
        throw std::runtime_error("too view arguments passed to mesh_converter");
    }
    devtools::mesh_converter mc(argv[1], argv[2]);
    if (argc == 4) {
        if (std::string{argv[3]} == "-col") {
            mc.run(true);
        } else {
            throw std::runtime_error("unrecognized option");
        }
    } else {
        mc.run();
    }
    return 0;
}
