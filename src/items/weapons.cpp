#include "weapons.hpp"
#include "entities/character.hpp"
#include <iostream>

// src/items/weapons.cpp
void Rifle::use(Character& user, Map& map) {
    if (ammo <= 0) return;
    ammo--;

    Position p = user.getPos();
    // The Weapon now controls the "Line of Sight" logic
    for (int i = 1; i <= range; i++) {
        Tile* targetTile = map.getTileAt(p.x + i, p.y);
        
        if (targetTile && targetTile->hasEnemy) {
            targetTile->hasEnemy = false; // Neutralize
            targetTile->updateSymbol();
            std::cout << "HIT!" << std::endl;
            return; // Stop the bullet
        }
    }
    std::cout << "MISS!" << std::endl;
}