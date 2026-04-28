// items/weapons.hpp
#pragma once
#include "item.hpp"

enum class DamageType { NONE, EXACT, LINE, RADIUS };

struct DamageRequest {
    DamageType type = DamageType::NONE;
    int damage = 0;
    float range = 0.0f;
};

struct ActionResult {
    bool itemWasDestroyed = false;
    DamageRequest damageReq;
};

class Gun : public DurableItem {
protected:
    int load;
    int maxLoad;
    float accuracy;
    int damage;
    float range;

public:
    // Pass a default name "Rifle" up to the Item constructor
    Gun(std::string Name, float weightInit, int loadInit, int maxLoadInit, float accuracyInit, int damageInit, float rangeInit, double betaInit, double etaInit)
        : DurableItem(Name, weightInit, betaInit, etaInit), maxLoad(maxLoadInit), load(loadInit), accuracy(accuracyInit), damage(damageInit), range(rangeInit)
    {};

    //bool use(User& user, Map& map) override;
    virtual ActionResult use(User& user) = 0;
    void reload();
};


// class halfAutoRifle : public Gun{
// public:
//     halfAutoRifle(std::string Name, int loadInit, int maxLoadInit, float accuracyInit, int damageInit, float rangeInit) 
//         : Gun(Name, maxLoadInit, loadInit, accuracyInit,damageInit, rangeInit)
//     {};
// };