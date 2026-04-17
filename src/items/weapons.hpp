// src/items/weapons.hpp
#pragma once
#include "item.hpp"
#include "../entities/character.hpp"
#include "../world/terrain.hpp"
#include <iostream>

class Rifle : public Item {
public:
    Rifle() : Item("Gewehr 98") {}
    void use(Player& player, Map& map) override {
        Position p = player.pos;
        for (int i = 1; i <= 5; i++) { // Range of 5
            Position t = player.getTargetPos(i);
            Tile* tile = map.getTile(t.x, t.y);
            if (tile && tile->hasZombie) {
                tile->hasZombie = false; // Kill
                return;
            }
        }
    }
};