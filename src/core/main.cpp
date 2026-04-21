#include "core/engine.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {

    try {
        std::srand(std::time(nullptr));
        Engine game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Engine Crash: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Game Over. Thank you for playing!" << std::endl;
    return 0;
}