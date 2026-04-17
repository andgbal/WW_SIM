#include "tools.hpp"
#include "../entities/character.hpp"
#include <iostream>

void Shovel::use(Character& user, Map& map) {
    Position p = user.getPos();
    map.grid[p.y][p.x].type = TerrainType::TRENCH;
    map.grid[p.y][p.x].updateSymbol();
    std::cout << "Action: Dug a trench!\\n";
}

void HealKit::use(Character& user, Map& map) {
    user.setHealth(user.getHealth() + 20);
    std::cout << "Action: Used bandages!\\n";
}
