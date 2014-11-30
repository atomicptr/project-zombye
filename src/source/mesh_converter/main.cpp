#include <stdexcept>
#include <string>

#include <mesh_converter/mesh_converter.hpp>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        throw std::invalid_argument("invalid argument");
    }
    devtools::mesh_converter mc(argv[1], argv[2]);
    bool collision_geometry = false;
    if (argc >= 4){
        if (std::string{argv[3]} == "-c") {
            collision_geometry = true;
        }
    }
    mc.parse(collision_geometry);
    mc.serialize(collision_geometry);
    return 0;
}