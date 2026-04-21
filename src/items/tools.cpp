// src/items/tools.cpp
#include "tools.hpp"
#include "../entities/character.hpp"
#include "../world/terrain.hpp"
#include <iostream>

Shovel::Shovel() : Item("Shovel") {}

void Shovel::use(Player& player, Map& map) {
    Tile* tile = map.getTile(player.pos.x, player.pos.y);
    if (!tile) return;

    tile->type = TerrainType::TRENCH;
    // TODO: call tile->updateSymbol() once you implement it
    std::cout << "Action: Dug a trench!\n";
}

HealthKit::HealthKit() : Item("HealthKit") {}

void HealthKit::use(Player& player, Map& map) {
    // TODO: once Player has health, do player.health += 20;
    
    std::cout << "Action: Used bandages! +20 HP\n";
}