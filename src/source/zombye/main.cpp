#include <iostream>
#include <string>

#include <zombye/core/game.hpp>
#include <zombye/utils/os.h>

int main() {
    std::cout << "init zombye with OS: " << OS_NAME << std::endl;

    zombye::game game("project-zombye", 800, 600);

    game.run();

    return 0;
}