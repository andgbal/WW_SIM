#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>
#include "Item.hpp" // Assuming your Item class is defined here

class Inventory {
private:
    std::vector<Item*> slots;
    float currentWeight;
    const float MAX_WEIGHT;

public:
    // Constructor
    Inventory(float maxWeightLimit = 50.0f);

    // Methods
    bool checkIfAdd(Item* item);
    Item* drop(int idx); // Returning the pointer so the Map can catch it
    Item* get(int idx) const;
    int count() const;
    float getWeight() const;
};

#endif