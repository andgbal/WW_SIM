// src/core/engine.hpp
#pragma once
#include "../world/terrain.hpp"
#include "../entities/character.hpp"

enum class GameState { PLAYING, INVENTORY };

class Engine {
private:
    Map map;
    User user;
    std::vector<Zombie> zombies;
    GameState state;
    int selectedItemIdx;
    bool running;

    void tick(); // Advances the round
    void render();
    void handleInput();

public:
    Engine();
    void run();
};