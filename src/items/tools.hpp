// src/items/tools.hpp
#pragma once
#include "item.hpp"
#include "../entities/character.hpp"
#include "../world/terrain.hpp"

class Shovel : public Item {
public:
    Shovel() : Item("Trench Shovel") {}
    void use(Player& player, Map& map) override {
        Position target = player.getTargetPos();
        Tile* t = map.getTile(target.x, target.y);
        if (t) t->type = TerrainType::TRENCH;
    }
};