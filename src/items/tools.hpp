// src/items/tools.hpp
#pragma once
#include "item.hpp"
#include "../entities/character.hpp"
#include "../world/terrain.hpp"

class Shovel : public Item {
public:
    Shovel();                    // calls Item("Shovel")
    void use(User& user, Map& map) override;
};

class HealthKit : public Item {
public:
    HealthKit();                    // calls Item("Shovel")
    void use(User& user, Map& map) override;
};