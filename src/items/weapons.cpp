// items/weapons.cpp
#include "weapons.hpp"
#include "../world/terrain.hpp"
#include "../entities/character.hpp"    // Player (pos + facing)

Rifle::Rifle() : Item("Rifle") {}

void Rifle::use(User& user, Map& map) {
    int dx = 0, dy = 0;
    switch (user.facing) {
        case Dir::NORTH: dy = -1; break;
        case Dir::SOUTH: dy =  1; break;
        case Dir::EAST:  dx =  1; break;
        case Dir::WEST:  dx = -1; break;
    }

    // Shoot up to 10 tiles straight ahead
    for (int i = 1; i <= 10; ++i) {
        int tx = user.pos.x + dx * i;
        int ty = user.pos.y + dy * i;

        Tile* tile = map.getTile(tx, ty);
        if (!tile) break;

        if (tile->hasZombie) {
            tile->hasZombie = false;   // kill zombie (map status only)
            break;
        }
    }
}