#include "item.hpp"
#include "../core/model.hpp"

float Item::getWeight(){return weight;}
std::string Item::getName(){return name;}

bool DurableItem::checkWillBreakOnThisUse() {
    return Model::willBreakOnNextUse(static_cast<double>(uses), beta, eta);
}