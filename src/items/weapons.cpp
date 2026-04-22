// items/weapons.cpp
#include "weapons.hpp"
#include "../world/terrain.hpp"
#include "../entities/character.hpp"
#include <cstdlib>   // rand()
#include <iostream>

void Gun::use(User& user, Map& map) {
    // 1. Ammo check
    if (load <= 0) {
        std::cout << name << ": *click* Out of ammo! Press R to reload.\n";
        return;
    }
    load--;

    // 2. Accuracy roll  (0.85 accuracy = 85% hit chance)
    if ((std::rand() % 100) >= static_cast<int>(accuracy * 100)) {
        std::cout << name << ": Shot missed! (" << load << "/" << maxLoad << " left)\n";
        return;
    }

    // 3. Determine step direction from player facing
    int dx = 0, dy = 0;
    switch (user.facing) {
        case Dir::NORTH: dy = -1; break;
        case Dir::SOUTH: dy =  1; break;
        case Dir::EAST:  dx =  1; break;
        case Dir::WEST:  dx = -1; break;
    }

    // 4. Walk forward up to range, hit first zombie found
    for (int i = 1; i <= range; ++i) {
        int tx = user.pos.x + dx * i;
        int ty = user.pos.y + dy * i;

        Tile* tile = map.getTile(tx, ty);
        if (!tile) break;                  // out of bounds — bullet stops

        if (tile->hasZombie) {
            tile->hasZombie = false;       // kill (map data only for now)
            std::cout << name << ": Hit! Zombie killed at (" << tx << "," << ty
                      << ")  [" << load << "/" << maxLoad << " ammo]\n";
            return;
        }
    }

    std::cout << name << ": Shot fired, nothing in range. ("
              << load << "/" << maxLoad << " left)\n";
}

void Gun::reload() {
    load = maxLoad;
    std::cout << name << ": Reloaded! (" << maxLoad << "/" << maxLoad << ")\n";
}

// ── Single shot, long range, high damage ──
class Rifle : public Gun {
public:
    Rifle() : Gun("Rifle", 5, 5, 0.85f, 80, 12) {}
};

// ── Burst-style: fires 3 shots per use, lower accuracy ──
class HalfAutoRifle : public Gun {
public:
    HalfAutoRifle() : Gun("HalfAutoRifle", 15, 15, 0.65f, 60, 10) {}
    void use(User& user, Map& map) override;  // fires 3 rounds at once
};

// ── Sidearm: small, low range, fast reload ──
class Pistol : public Gun {
public:
    Pistol() : Gun("Pistol", 8, 8, 0.70f, 40, 6) {}
};