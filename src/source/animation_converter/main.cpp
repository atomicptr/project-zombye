#include <stdexcept>

#include <animation_converter/animation_converter.hpp>

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        throw std::invalid_argument("invalid argument");
    }
    devtools::animation_converter ac(argv[1], argv[2]);
    if (argc == 4) {
        ac.parse(argv[3]);
    } else {
        ac.parse("");
    }
    ac.serialize();
    return 0;
}