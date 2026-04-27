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

// ── Type 1: Consumable (always breaks after one use) ──
class ConsumableItem : public Item {
public:
    ConsumableItem(std::string n, float w) : Item(n, w) {}

    // Derived classes implement the actual effect, then return true
    bool use(User& user, Map& map) override = 0;
};

// ── Type 2: Durable / Repeatable (Weibull endurance) ──
class DurableItem : public Item {
protected:
    int uses = 0;           // times used so far
    double beta = 3.0;      // shape parameter (>1 = wear-out)
    double eta  = 60.0;     // scale (characteristic life in uses)

public:
    DurableItem(std::string n, float w, double betaInit = 3.0, double etaInit = 60.0)
        : Item(n, w), beta(betaInit), eta(etaInit) {}

    // Derived classes implement effect, then call checkEndurance()
    bool use(User& user, Map& map) override = 0;

protected:
    bool checkWillBreakOnThisUse();   // pre-use check
    void incrementUses() { ++uses; }
};