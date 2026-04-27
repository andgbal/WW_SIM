#include "inventory.hpp"
#include <iostream>


// Constructor initialization
Inventory::Inventory(float maxWeightLimit) 
    : currentWeight(0.0f), MAX_WEIGHT(maxWeightLimit) {}

bool Inventory::checkIfAdd(Item* item) {
    if (item == nullptr) return false;

    // Follow the pointer to the Warehouse to check the item's weight
    if (currentWeight + item->getWeight() <= MAX_WEIGHT) {
        slots.push_back(item);
        currentWeight += item->getWeight();
        return true;
    }

    std::cout << "Inventory: Too heavy to add " << item->getName() << "!" << std::endl;
    return false;
}

Item* Inventory::drop(int idx) {
    if (idx < 0 || idx >= static_cast<int>(slots.size())) {
        return nullptr;
    }

    Item* itemToDrop = slots[idx];
    currentWeight -= itemToDrop->getWeight();
    
    // Remove the "Business Card" from the vector
    slots.erase(slots.begin() + idx);

    return itemToDrop; // The Map will now "own" this pointer
}

Item* Inventory::get(int idx) const {
    if (idx < 0 || idx >= static_cast<int>(slots.size())) {
        return nullptr;
    }
    return slots[idx];
}

int Inventory::count() const {
    return static_cast<int>(slots.size());
}

float Inventory::getWeight() const {
    return currentWeight;
}