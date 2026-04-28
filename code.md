## File: src\core\engine.cpp
```
#include "engine.hpp"
#include "../items/tools.hpp"
#include "../items/weapons.hpp"
#include <iostream>
#include <windows.h>

//Spawning algo function required
void Engine::spawnInitialItems() {
    // Optional: spawn more randomly
    for (int i = 0; i < 10; ++i) {
        int x = rand() % map.width;
        int y = rand() % map.height;
        if (std::abs(x - 25) + std::abs(y - 25) > 8) {  // not too close to player
            Item* item = nullptr;
            int r = rand() % 4;
            if (r == 0)      item = spawnItem<Rifle>();
            else if (r == 1) item = spawnItem<Pistol>();
            else if (r == 2) item = spawnItem<Bandage>();
            else             item = spawnItem<KitBox>();

            if (item) map.placeItem({x, y}, item);
        }
    }
}


Engine::Engine() : user(25, 25), state(GameState::PLAYING), selectedItemIdx(0), running(true) {
    // Hide cursor for cleanliness
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    //handle item spawn here
    spawnInitialItems();















    
    for (int i = 0; i < 30; i++) {
        int zx = rand() % map.width;
        int zy = rand() % map.height;
        if (zx != 25 || zy != 25) {
            zombies.push_back(Zombie(zx, zy));
            map.getTile(zx, zy)->hasZombie = true;
        }
    }
}



void Engine::tick() {

    for (auto& z : zombies) {
        z.update(map, user.pos);
    }

    Item* found = map.takeItem(user.pos);
    if (found) {
        bool success = user.addToInventory(found);
        if (!success)
            map.placeItem(user.pos, found);  // too heavy, put back
    }

}

void Engine::render() {
    system("cls"); 

    std::cout << "--- TRENCH ZOMBIES | STABLE VERSION ---\n";
    
    if (state == GameState::INVENTORY) {
        std::cout << "\n  [ BACKPACK ]  " 
                << user.storage.getWeight() << " / " << user.maxWeight << " kg\n";
        std::cout << "  W/S: Scroll | F: Use | X: Drop | Q: Close\n";
        std::cout << "  ------------------------------\n";

        if (user.storage.count() == 0) {
            std::cout << "   (empty)\n";
        } else {
            for (int i = 0; i < user.storage.count(); i++) {
                Item* item = user.storage.get(i);
                std::cout << (i == selectedItemIdx ? " > " : "   ")
                        << item->getName()
                        << "  " << item->getWeight() << "kg\n";
            }
        }
        return;
    }

    std::cout << "Controls: WASD (Move) | IJKL (Turn) | E (Menu)\n";
    std::cout << "Current Action: Waiting for input...\n\n";
    
    // Camera Viewport Settings
    int viewH = 6; 
    int viewW = 12;

    for (int y = user.pos.y - viewH; y <= user.pos.y + viewH; y++) {
        for (int x = user.pos.x - viewW; x <= user.pos.x + viewW; x++) {
            
            // 1. Draw Player (Centered)
            if (x == user.pos.x && y == user.pos.y) {
                if(user.facing == Dir::NORTH)      std::cout << "^ ";
                else if(user.facing == Dir::SOUTH) std::cout << "v ";
                else if(user.facing == Dir::EAST)  std::cout << "> ";
                else                                 std::cout << "< ";
            } 
            // 2. Draw Boundaries
            else if (x < 0 || x >= map.width || y < 0 || y >= map.height) {
                std::cout << "X "; 
            }
            // 3. Draw Vision Cone (Light)
            else if (map.inVisionCone(user.pos, user.facing, x, y, 8)) {
                // Draw whatever is on the tile (Enemy 'E', Trench '#', or Dirt '.')
                std::cout << map.getTile(x, y)->getSymbol() << " ";
            } 
            // 4. Draw SOLID DARKNESS (Fog)
            else {
                // Switched to '▒' (char 177) or '#' for maximum compatibility
                // Let's use '▒' style dots or simple '#'
                std::cout << "## "; 
            }
        }
        std::cout << "\n";
    }
}

void Engine::handleInput() {
    char in;
    std::cin >> in; // Back to standard buffered input

    bool tookAction = false;

    if (state == GameState::PLAYING) {
        if (in == 'w' && user.pos.y > 0) { user.pos.y--; tookAction = true; }
        else if (in == 's' && user.pos.y < map.height - 1) { user.pos.y++; tookAction = true; }
        else if (in == 'a' && user.pos.x > 0) { user.pos.x--; tookAction = true; }
        else if (in == 'd' && user.pos.x < map.width - 1) { user.pos.x++; tookAction = true; }
        else if (in == 'i') { user.facing = Dir::NORTH; tookAction = true; }
        else if (in == 'k') { user.facing = Dir::SOUTH; tookAction = true; }
        else if (in == 'j') { user.facing = Dir::WEST;  tookAction = true; }
        else if (in == 'l') { user.facing = Dir::EAST;  tookAction = true; }
        else if (in == 'e') state = GameState::INVENTORY;
        else if (in == 'q') running = false;
    } 
    else if (state == GameState::INVENTORY) {
        if (in == 'w' && selectedItemIdx > 0) selectedItemIdx--;
        else if (in == 's' && selectedItemIdx < (int)user.storage.count() - 1) selectedItemIdx++;
        else if (in == 'q') state = GameState::PLAYING;
        else if (in == 'f') {
            Item* selected = user.storage.get(selectedItemIdx);
            bool broken = selected->use(user, map);

            if (broken) {
                user.removeFromInventory(selectedItemIdx);  // step 1: out of inventory
                destroyItem(selected);                       // step 2: deleted from heap
                selected = nullptr;                          // step 3: don't leave a live var pointing to freed memory

                if (selectedItemIdx >= (int)user.storage.count())
                    selectedItemIdx = (int)user.storage.count() - 1;
                if (selectedItemIdx < 0) selectedItemIdx = 0;
            }

            state = GameState::PLAYING;
            tookAction = true;
        }
        else if (in == 'x'){
            Item* dropped = user.removeFromInventory(selectedItemIdx);
            map.placeItem(user.pos, dropped);


                if (selectedItemIdx >= (int)user.storage.count())
                    selectedItemIdx = (int)user.storage.count() - 1;
                if (selectedItemIdx < 0) selectedItemIdx = 0;
        }
    }

    if (tookAction) tick();
}

void Engine::run() {
    while (running) {
        render();
        handleInput();
        // No Sleep needed because cin pauses the execution naturally
    }
}
```

## File: src\core\engine.hpp
```
// src/core/engine.hpp
#pragma once
#include "../world/terrain.hpp"
#include "../entities/character.hpp"
#include <vector>
#include <memory>

enum class GameState { PLAYING, INVENTORY };

class Engine {
private:
    Map map;
    User user;
    std::vector<std::unique_ptr<Item>> masterItemList;

    // Allocates item, registers in master list, returns raw pointer
    template<typename T, typename... Args>
    Item* spawnItem(Args&&... args) {
        masterItemList.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        return masterItemList.back().get();
    }

    // Removes from master list — caller must have already removed it from
    // inventory or groundItems before calling this
    void destroyItem(Item* item) {
        masterItemList.erase(
            std::remove_if(masterItemList.begin(), masterItemList.end(),
                [item](const std::unique_ptr<Item>& p){ return p.get() == item; }),
            masterItemList.end()
        );
    }

    void spawnInitialItems();

    GameState state;
    int selectedItemIdx;
    bool running;

    void tick(); // Advances the round
    void render();
    void handleInput();

public:
    Engine();
    void run();
};
```

## File: src\core\inventory.cpp
```
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
```

## File: src\core\inventory.hpp
```
#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>
#include "../items/item.hpp" // Assuming your Item class is defined here

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
```

## File: src\core\main.cpp
```
#include "core/engine.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {

    try {
        std::srand(std::time(nullptr));
        Engine game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Engine Crash: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Game Over. Thank you for playing!" << std::endl;
    return 0;
}
```

## File: src\core\model.cpp
```
#include "model.hpp"
#include <cmath>
#include <vector>

namespace Model {

    // Item endurance calculation

    double weibullSurvival(double t, double beta, double eta) {
        if (t <= 0.0) return 1.0;
        if (eta <= 0.0) return 0.0;  // invalid
        return std::exp( -std::pow(t / eta, beta) );
    }

    double weibullBreakProbability(double t, double beta, double eta) {
        double survival = weibullSurvival(t, beta, eta);
        return 1.0 - survival;
    }

    bool willBreakOnNextUse(double currentUses, double beta, double eta) {
        double breakProb = weibullBreakProbability(currentUses, beta, eta);
        double roll = static_cast<double>(std::rand()) / RAND_MAX;
        return roll < breakProb;
    }

    // damage system

    struct HitInfo {
        Position pos;
        int damageDealt;
    };

    DamageReport Model::processLineDamage(const DamageRequest& req, Map& map) {
        DamageReport report;
        int dx = 0, dy = 0;
        
        if (req.direction == Dir::NORTH) dy = -1;
        else if (req.direction == Dir::SOUTH) dy = 1;
        else if (req.direction == Dir::EAST)  dx = 1;
        else if (req.direction == Dir::WEST)  dx = -1;

        for (int i = 1; i <= static_cast<int>(req.range); ++i) {
            int tx = req.origin.x + (dx * i);
            int ty = req.origin.y + (dy * i);

            Tile* tile = map.getTile(tx, ty);
            if (!tile) break; // Out of bounds

            if (tile->hasZombie) {
                report.actualHits.push_back({{tx, ty}, req.damage});
                break; // Stop at first entity hit
            }
        }
        return report;
    }

    DamageReport Model::processRadialDamage(const DamageRequest& req, Map& map) {
        DamageReport report;
        int r = static_cast<int>(req.range);

        // Check a bounding box around the explosion
        for (int y = req.origin.y - r; y <= req.origin.y + r; y++) {
            for (int x = req.origin.x - r; x <= req.origin.x + r; x++) {
                
                // Circular distance check
                float dist = std::sqrt(std::pow(x - req.origin.x, 2) + std::pow(y - req.origin.y, 2));
                
                if (dist <= req.range) {
                    Tile* tile = map.getTile(x, y);
                    if (tile && tile->hasZombie) {
                        report.actualHits.push_back({{x, y}, req.damage});
                    }
                }
            }
        }
        return report;
    }

} // namespace Model
```

## File: src\core\model.hpp
```
#pragma once
#include "../core/engine.hpp"
#include <cmath>

namespace Model {

    // Weibull Survival Function: Probability that item still works after t uses
    // R(t) = exp( - (t / η)^β )
    double weibullSurvival(double t, double beta, double eta);

    // Break probability after this use: 1 - R(t)
    double weibullBreakProbability(double t, double beta, double eta);

    // Convenience: decide if item breaks after current use
    bool willBreakOnNextUse(double currentUses, double beta, double eta);

    // Damage system
    enum class DamageType { LINE, CIRCLE, RECTANGLE };

    struct DamageRequest {
        Position origin;
        Dir direction;
        int damage;
        float range;      // Radius for circle, length for line
        float width;      // Used for rectangles or cones
        DamageType type;
    };

    struct DamageReport {
        struct Hit {
            Position pos;
            int damageDealt;
        };
        std::vector<Hit> actualHits; // Only tiles where an entity was found
    };

} // namespace Model
```

## File: src\entities\character.cpp
```
// src/entities/character.cpp
#include "character.hpp"
#include <cmath>

int Character::getHealth(){
    return health;
}

void Character::setHealth(int h){
    health = h;
}

Position User::getTargetPos(int d) {
    if (facing == Dir::NORTH) return {pos.x, pos.y - d};
    if (facing == Dir::SOUTH) return {pos.x, pos.y + d};
    if (facing == Dir::EAST)  return {pos.x + d, pos.y};
    return {pos.x - d, pos.y};
}


bool User::addToInventory(Item* item){
    return storage.checkIfAdd(item);
}

Item* User::removeFromInventory(int idx){
    return storage.drop(idx);
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
```

## File: src\entities\character.hpp
```
// src/entities/character.hpp
#pragma once
#include "../world/terrain.hpp"
#include "../items/item.hpp"
#include "../core/inventory.hpp"
#include <vector>

class Character {
public:
    Character(int x, int y, int h) : pos({x, y}), health(h) {}
    Position pos;    
    int getHealth();
    void setHealth(int h);
protected:
    int health;
};

class User : public Character {
public:
    Dir facing = Dir::EAST;
    Inventory storage;
    float maxWeight;

    User(int x, int y, int h = 100, float maxWeightInit = 15.0f) : Character(x, y, h), maxWeight(maxWeightInit), storage(maxWeightInit) {}
    Position getTargetPos(int distance = 1);

    bool addToInventory(Item* item);      // weight check, then push_back
    Item* removeFromInventory(int idx);   // erase + return pointer
};
























class Zombie : public Character {
public:
    int detectRadius = 6;
    Zombie(int x, int y, int h = 50) : Character(x, y, h) {}
    void update(Map& map, Position playerPos);
};
```

## File: src\items\item.cpp
```
#include "item.hpp"
#include "../core/model.hpp"

float Item::getWeight(){return weight;}
std::string Item::getName(){return name;}

bool DurableItem::checkWillBreakOnThisUse() {
    return Model::willBreakOnNextUse(static_cast<double>(uses), beta, eta);
}
```

## File: src\items\item.hpp
```
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
    virtual bool use(User& user) = 0;

protected:
    bool checkWillBreakOnThisUse();   // pre-use check
    void incrementUses() { ++uses; }
};
```

## File: src\items\tools.cpp
```
// src/items/tools.cpp
#include "tools.hpp"
#include "../entities/character.hpp"
#include "../world/terrain.hpp"
#include <iostream>

// Shovel::Shovel() : Item("Shovel") {}

// void Shovel::use(User& user, Map& map) {
//     Tile* tile = map.getTile(user.pos.x, user.pos.y);
//     if (!tile) return;

//     tile->type = TerrainType::TRENCH;
//     // TODO: call tile->updateSymbol() once you implement it
//     std::cout << "Action: Dug a trench!\n";
// }

bool HealthKit::use(User& user, Map& map){
    user.setHealth(user.getHealth() + healthRecover);
    return true;
}

class Bandage : public HealthKit{
public:
    Bandage() : HealthKit("Bandage", 1.0f, 5) {}
};

class KitBox : public HealthKit{
public:
    KitBox() : HealthKit("kitBox", 2.0f, 20) {}
};

class MedicalBox : public HealthKit{
public:
    MedicalBox() : HealthKit("MedicalBox", 5.0f, 20) {}
};
```

## File: src\items\tools.hpp
```
// src/items/tools.hpp
#pragma once
#include "item.hpp"
#include "../entities/character.hpp"
#include "../world/terrain.hpp"

// class Shovel : public Item {
// public:
//     Shovel();                    // calls Item("Shovel")
//     void use(User& user, Map& map) override;
// };

class HealthKit : public Item {
protected:
    int healthRecover;
public:
    HealthKit(std::string name, float weightInit, int healthRecoverInit) : 
        Item(name, weightInit), healthRecover(healthRecoverInit) {};
    bool use(User& user, Map& map) override;
};
```

## File: src\items\weapons.cpp
```
// items/weapons.cpp
#include "weapons.hpp"
#include "../world/terrain.hpp"
#include "../entities/character.hpp"
#include <cstdlib>   // rand()
#include <iostream>

bool Gun::use(User& user, Map& map) {
    // 1. Ammo check
    if (load <= 0) {
        std::cout << name << ": *click* Out of ammo! Press R to reload.\n";
        return;
    }
    load--;

    // 2. Accuracy roll  (0.85 accuracy = 85% hit chance)
    if ((std::rand() % 100) >= static_cast<int>(accuracy * 100)) {
        std::cout << name << ": Shot missed! (" << load << "/" << maxLoad << " left)\n";
        return;
    }

    // 3. Determine step direction from player facing
    int dx = 0, dy = 0;
    switch (user.facing) {
        case Dir::NORTH: dy = -1; break;
        case Dir::SOUTH: dy =  1; break;
        case Dir::EAST:  dx =  1; break;
        case Dir::WEST:  dx = -1; break;
    }

    // 4. Walk forward up to range, hit first zombie found
    for (int i = 1; i <= range; ++i) {
        int tx = user.pos.x + dx * i;
        int ty = user.pos.y + dy * i;

        Tile* tile = map.getTile(tx, ty);
        if (!tile) break;                  // out of bounds — bullet stops

        if (tile->hasZombie) {
            tile->hasZombie = false;       // kill (map data only for now)
            std::cout << name << ": Hit! Zombie killed at (" << tx << "," << ty
                      << ")  [" << load << "/" << maxLoad << " ammo]\n";
            return;
        }
    }

    std::cout << name << ": Shot fired, nothing in range. ("
              << load << "/" << maxLoad << " left)\n";
}

void Gun::reload() {
    load = maxLoad;
    std::cout << name << ": Reloaded! (" << maxLoad << "/" << maxLoad << ")\n";
}

// ── Single shot, long range, high damage ──
class Rifle : public Gun {
public:
    Rifle() : Gun("Rifle", 5.0f, 5, 5, 0.85f, 80, 12) {}
};

// ── Burst-style: fires 3 shots per use, lower accuracy ──
class HalfAutoRifle : public Gun {
public:
    HalfAutoRifle() : Gun("HalfAutoRifle", 5.0f, 15, 15, 0.65f, 60, 10) {}
    bool use(User& user, Map& map) override;  // fires 3 rounds at once
};

// ── Sidearm: small, low range, fast reload ──
class Pistol : public Gun {
public:
    Pistol() : Gun("Pistol", 3.0f, 8, 8, 0.70f, 40, 6) {}
};
```

## File: src\items\weapons.hpp
```
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
```

## File: src\world\terrain.cpp
```
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

void Map::placeItem(Position pos, Item* item) {
    groundItems.push_back({pos, item});
    Tile* tile = getTile(pos.x, pos.y);
    if (tile) tile->groundItem = item;      // sync for rendering
}

Item* Map::takeItem(Position pos) {
    for (auto it = groundItems.begin(); it != groundItems.end(); ++it) {
        if (it->pos.x == pos.x && it->pos.y == pos.y) {
            Item* p = it->item;
            groundItems.erase(it);
            Tile* tile = getTile(pos.x, pos.y);
            if (tile) tile->groundItem = nullptr;  // sync for rendering
            return p;
        }
    }
    return nullptr;
}

Item* Map::peekItem(Position pos) const {
    for (const auto& g : groundItems) {
        if (g.pos.x == pos.x && g.pos.y == pos.y)
            return g.item;
    }
    return nullptr;
}
```

## File: src\world\terrain.hpp
```
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
```

