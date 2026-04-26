// src/items/item.hpp
#pragma once
#include <string>
class User; // Forward declaration
class Map;

class Item {
public:
    std::string name;
    float weight;

    // No default constructor → forces derived classes to set a name
    explicit Item(std::string n, float weightInit) : name(std::move(n)), weight(weightInit) {}

    virtual ~Item() = default;
    virtual bool use(User& user, Map& map) = 0;

    float getWeight();
    std::string getName();
    
};