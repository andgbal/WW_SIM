// src/items/item.hpp
#pragma once
#include <string>
class Player; // Forward declaration
class Map;

class Item {
public:
    std::string name;
    Item(std::string n) : name(n) {}
    virtual ~Item() = default;
    virtual void use(Player& player, Map& map) = 0;
};