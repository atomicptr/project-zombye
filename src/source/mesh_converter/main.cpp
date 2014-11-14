#include <stdexcept>

#include <mesh_converter/mesh_converter.hpp>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        throw std::invalid_argument("invalid argument");
    }
    devtools::mesh_converter mc(argv[1], argv[2]);
    mc.parse();
    mc.serialize();
    return 0;
}