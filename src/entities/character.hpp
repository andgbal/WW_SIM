// src/entities/character.hpp
#pragma once
#include "../world/terrain.hpp"
#include "../items/item.hpp"
#include <vector>

class Character {
public:
    Character(int x, int y, int h) : pos({x, y}), health(h) {}
    Position pos;    
    int getHealth() {};
    void setHealth(int h) {};
protected:
    int health;
};

class User : public Character {
public:
    Dir facing = Dir::EAST;
    std::vector<Item*> inventory;
    float currentWeight;
    float maxWeight;

    User(int x, int y, int h = 100, float maxWeightInit = 15.0f) : Character(x, y, h), maxWeight(maxWeightInit) {}
    Position getTargetPos(int distance = 1);

    bool pickUpItem(Item* item) {}; //? what is this delete if not used
    bool addToInventory(Item* item);      // weight check, then push_back
    Item* removeFromInventory(int idx);   // erase + return pointer
};
























class Zombie : public Character {
public:
    int detectRadius = 6;
    Zombie(int x, int y, int h = 50) : Character(x, y, h) {}
    void update(Map& map, Position playerPos);
};