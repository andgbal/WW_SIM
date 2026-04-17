// items/weapons.cpp
#include "weapons.hpp"
#include "terrain.hpp"   // Map, Tile
#include "../entities/character.hpp"    // Player (pos + facing)

Rifle::Rifle() {
    name = "Rifle";
}

void Rifle::use(Player& player, Map& map) {
    int dx = 0, dy = 0;
    switch (player.facing) {
        case Dir::NORTH: dy = -1; break;
        case Dir::SOUTH: dy =  1; break;
        case Dir::EAST:  dx =  1; break;
        case Dir::WEST:  dx = -1; break;
    }

    // Shoot up to 10 tiles straight ahead
    for (int i = 1; i <= 10; ++i) {
        int tx = player.pos.x + dx * i;
        int ty = player.pos.y + dy * i;

        Tile* tile = map.getTile(tx, ty);
        if (!tile) break;

        if (tile->hasZombie) {
            tile->hasZombie = false;   // kill zombie (map status only)
            break;
        }
    }
}