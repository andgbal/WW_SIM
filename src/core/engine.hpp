// src/core/engine.hpp
#pragma once
#include "../world/terrain.hpp"
#include "../entities/character.hpp"
#include <vector>
#include <memory>

enum class GameState { PLAYING, INVENTORY };

class Engine {
private:
    Map map;
    User user;
    std::vector<std::unique_ptr<Item>> masterItemList;
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