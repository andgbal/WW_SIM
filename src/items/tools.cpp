// src/items/tools.cpp
#include "tools.hpp"
#include "../entities/character.hpp"
#include "../world/terrain.hpp"
#include <iostream>

Shovel::Shovel() : Item("Shovel") {}

void Shovel::use(User& user, Map& map) {
    Tile* tile = map.getTile(user.pos.x, user.pos.y);
    if (!tile) return;

    tile->type = TerrainType::TRENCH;
    // TODO: call tile->updateSymbol() once you implement it
    std::cout << "Action: Dug a trench!\n";
}

HealthKit::HealthKit() : Item("HealthKit") {}

void HealthKit::use(User& user, Map& map) {
    // TODO: once Player has health, do player.health += 20;
    int old = user.getHealth();
    user.setHealth(old + 20);
    //std::cout << "Action: Used bandages! +20 HP\n";
}