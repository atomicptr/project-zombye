#include <stdexcept>

#include <mesh_converter/mesh_converter.hpp>

int main(int argc, char const* argv[]) {
    if (argc < 3) {
        throw std::runtime_error("too view arguments passed to mesh_converter");
    }
    devtools::mesh_converter mc(argv[1], argv[2]);
    mc.run();
    return 0;
}