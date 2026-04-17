// src/entities/character.hpp
#pragma once
#include "../world/terrain.hpp"
#include "../items/item.hpp"
#include <vector>

class Character {
public:
    Position pos;
    Character(int x, int y) : pos({x, y}) {}
};

class Player : public Character {
public:
    Dir facing = Dir::EAST;
    std::vector<Item*> inventory;

    Player(int x, int y) : Character(x, y) {}
    Position getTargetPos(int distance = 1);
};

class Zombie : public Character {
public:
    int detectRadius = 6;
    Zombie(int x, int y) : Character(x, y) {}
    void update(Map& map, Position playerPos);
};