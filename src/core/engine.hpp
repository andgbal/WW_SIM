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

    // Allocates item, registers in master list, returns raw pointer
    template<typename T, typename... Args>
    Item* spawnItem(Args&&... args) {
        masterItemList.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        return masterItemList.back().get();
    }

    // Removes from master list — caller must have already removed it from
    // inventory or groundItems before calling this
    void destroyItem(Item* item) {
        masterItemList.erase(
            std::remove_if(masterItemList.begin(), masterItemList.end(),
                [item](const std::unique_ptr<Item>& p){ return p.get() == item; }),
            masterItemList.end()
        );
    }

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