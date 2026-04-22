// items/weapons.hpp
#pragma once
#include "item.hpp"

class Rifle : public Item {
protected:
    int load;
    int maxLoad;
    float accurarcy;
    int damage;
    float range;

public:
    // Pass a default name "Rifle" up to the Item constructor
    Rifle(std::string Name, int loadInit, int maxLoadInit, float accurarcyInit, int damageInit, float rangeInit) 
        : Item(Name), maxLoad(maxLoadInit), load(loadInit), accurarcy(accurarcyInit), damage(damageInit), range(rangeInit)
    {};

    void use(User& user, Map& map) override;
    void reload(User& user);
};


class halfAutoRifle : public Rifle{
public:
    halfAutoRifle(std::string Name, int loadInit, int maxLoadInit, float accurarcyInit, int damageInit, float rangeInit) 
        : Rifle(Name, maxLoadInit, loadInit, accurarcyInit,damageInit, rangeInit)
    {};
};