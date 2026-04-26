#include "terrain.hpp"
#include"item.hpp"
#include <cstdlib>
#include <ctime>

Map::Map() {
    width = 50;  // Make the map huge
    height = 50;
    grid.resize(height, std::vector<Tile>(width));
    
    // Generate random terrain
    //std::srand(std::time(nullptr));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (std::rand() % 100 < 10) { // 10% chance for a trench
                grid[y][x].type = TerrainType::TRENCH;
            }
        }
    }
}

Tile* Map::getTile(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) return &grid[y][x];
    return nullptr;
}

bool Map::inVisionCone(Position p, Dir facing, int tx, int ty, int range) {
    int dx = tx - p.x;
    int dy = ty - p.y;
    if (std::abs(dx) + std::abs(dy) > range) return false;

    if (facing == Dir::NORTH) return dy <= 0 && std::abs(dx) <= std::abs(dy);
    if (facing == Dir::SOUTH) return dy >= 0 && std::abs(dx) <= dy;
    if (facing == Dir::EAST)  return dx >= 0 && std::abs(dy) <= dx;
    if (facing == Dir::WEST)  return dx <= 0 && std::abs(dy) <= std::abs(dx);
    return false;
}

void Map::placeItem(Position pos, Item* item){
    groundItems.push_back({pos, item});
}

Item* Map::takeItem(Position pos){
    for (auto it = groundItems.begin(); it != groundItems.end(); ++it) {
        if (it->pos.x == pos.x && it->pos.y == pos.y) {
            Item* p = it->item;       // grab the raw ptr
            groundItems.erase(it);    // remove the {pos, ptr} pair from map
            return p;                 // hand ptr to caller
        }
    }
    return nullptr;   // nothing on this tile
}