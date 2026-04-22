// items/weapons.hpp
#pragma once
#include "item.hpp"

class Gun : public Item {
protected:
    int load;
    int maxLoad;
    float accuracy;
    int damage;
    float range;

public:
    // Pass a default name "Rifle" up to the Item constructor
    Gun(std::string Name, int loadInit, int maxLoadInit, float accuracyInit, int damageInit, float rangeInit) 
        : Item(Name), maxLoad(maxLoadInit), load(loadInit), accuracy(accuracyInit), damage(damageInit), range(rangeInit)
    {};

    void use(User& user, Map& map) override;
    void reload();
};


// class halfAutoRifle : public Gun{
// public:
//     halfAutoRifle(std::string Name, int loadInit, int maxLoadInit, float accuracyInit, int damageInit, float rangeInit) 
//         : Gun(Name, maxLoadInit, loadInit, accuracyInit,damageInit, rangeInit)
//     {};
// };