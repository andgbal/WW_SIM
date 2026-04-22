// src/items/item.hpp
#pragma once
#include <string>
class User; // Forward declaration
class Map;

class Item {
public:
    std::string name;

    // No default constructor → forces derived classes to set a name
    explicit Item(std::string n) : name(std::move(n)) {}

    virtual ~Item() = default;
    virtual void use(User& user, Map& map) = 0;
    
};