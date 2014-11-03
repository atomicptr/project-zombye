#include <iostream>
#include <string>

#include <zombye/core/game.hpp>

int main() {
    zombye::game game("project-zombye", 800, 600);

    game.run();

    return 0;
}
