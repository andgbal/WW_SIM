// items/weapons.hpp
#pragma once
#include "item.hpp"

class Rifle : public Item {
public:
    Rifle();
    void use(Player& player, Map& map) override;
};