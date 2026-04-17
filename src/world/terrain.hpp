// src/world/terrain.hpp
#pragma once
#include <vector>
#include <cmath>

enum class TerrainType { DIRT, TRENCH };
enum class Dir { NORTH, SOUTH, EAST, WEST };
struct Position { int x, y; };

class Tile {
public:
    TerrainType type = TerrainType::DIRT;
    bool hasZombie = false;
    char getSymbol() const {
        if (hasZombie) return 'Z';
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
};