#include "core/engine.hpp"
#include <iostream>

int main() {

    try {
        Engine game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Engine Crash: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Game Over. Thank you for playing!" << std::endl;
    return 0;
}