// src/entities/character.cpp
#include "character.hpp"
#include <cmath>

int Character::getHealth(){
    return Character::health;
}

void Character::setHealth(int h){
    Character::health = h;
}

Position User::getTargetPos(int d) {
    if (facing == Dir::NORTH) return {pos.x, pos.y - d};
    if (facing == Dir::SOUTH) return {pos.x, pos.y + d};
    if (facing == Dir::EAST)  return {pos.x + d, pos.y};
    return {pos.x - d, pos.y};
}

void Zombie::update(Map& map, Position pPos) {
    int dist = std::abs(pos.x - pPos.x) + std::abs(pos.y - pPos.y);
    if (dist <= detectRadius && dist > 0) {
        // Clear current tile
        map.getTile(pos.x, pos.y)->hasZombie = false;
        
        // Move towards player
        if (pPos.x > pos.x) pos.x++;
        else if (pPos.x < pos.x) pos.x--;
        else if (pPos.y > pos.y) pos.y++;
        else if (pPos.y < pos.y) pos.y--;
        
        // Update new tile
        map.getTile(pos.x, pos.y)->hasZombie = true;
    }
}