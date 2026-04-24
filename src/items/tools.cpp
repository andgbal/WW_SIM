// src/items/tools.cpp
#include "tools.hpp"
#include "../entities/character.hpp"
#include "../world/terrain.hpp"
#include <iostream>

// Shovel::Shovel() : Item("Shovel") {}

// void Shovel::use(User& user, Map& map) {
//     Tile* tile = map.getTile(user.pos.x, user.pos.y);
//     if (!tile) return;

//     tile->type = TerrainType::TRENCH;
//     // TODO: call tile->updateSymbol() once you implement it
//     std::cout << "Action: Dug a trench!\n";
// }

void HealthKit::use(User& user, Map& map){
    user.setHealth(user.getHealth() + healthRecover);
}

class Bandage : public HealthKit{
public:
    Bandage() : HealthKit("Bandage", 1.0f, 5) {}
};

class KitBox : public HealthKit{
public:
    KitBox() : HealthKit("kitBox", 2.0f, 20) {}
};

class MedicalBox : public HealthKit{
public:
    MedicalBox() : HealthKit("MedicalBox", 5.0f, 20) {}
};