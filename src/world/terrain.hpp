// src/world/terrain.hpp
#pragma once
#include <vector>
#include <cmath>

enum class TerrainType { DIRT, TRENCH };
enum class Dir { NORTH, SOUTH, EAST, WEST };
struct Position { int x, y; };

class Item;

class Tile {
public:
    TerrainType type = TerrainType::DIRT;
    Item* groundItem = nullptr;
    bool hasZombie = false;
    char getSymbol() const {
        if (hasZombie) return 'Z';
        if (groundItem) return 'i';
        if (type == TerrainType::TRENCH) return '#';
        return '.';
    }
};

class Map {
public:
    int width = 20, height = 10;
    std::vector<std::vector<Tile>> grid;

    Map();
    Tile* getTile(int x, int y);
    bool inVisionCone(Position p, Dir facing, int tx, int ty, int range);

    // In Map class:
    struct GroundItem {
        Position pos;
        Item* item;   // non-owning
    };
    std::vector<GroundItem> groundItems;

    void placeItem(Position pos, Item* item);   // add to groundItems
    Item* takeItem(Position pos);               // remove + return pointer (or nullptr)
    Item* peekItem(Position pos) const;         // check without removing
};