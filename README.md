# Summary
project for 2d round base survival simulator c++:
background ww1 kinds of single rifle, controlling single person
item on ground, mines, rile differner status (shoot range, aim %, broken rate), genereade, heal box, reload, usable item: shovel (create wall by digging, torch kinds of things for futher visual, talkie for calling support from air bombing)

different terrain generation

# File Structure

│  README.md
│  
└─src
    ├─core
    │      engine.cpp
    │      engine.hpp
    │      main.cpp
    │      
    ├─entities
    │      character.cpp
    │      character.hpp
    │      
    ├─items
    │      item.hpp
    │      tools.cpp
    │      tools.hpp
    │      weapons.cpp
    │      weapons.hpp
    │      
    └─world
            terrain.cpp
            terrain.hpp

# Map rendering
Terrain:

- river
- slope
- hole
- bush
- dirt

with different item spwanr rate and effecto to character

Enemy

# Character
Player, Enemy

health, move rate, storage

Enemy: storage, helath, movement

# Item
Endurance, Weight

## Weapon
shoot range, aim ability, damage
## Tools
shovel, light item for visual clearnace, heal box,, ammo, 

function

# Concept of each item connector
Engine (game loop + render + input)
   │
   ├── Player.inventory  →  std::vector<Item*>
   │
   └── calls → Item::use(Player& player, Map& map)
                │
                ▼
           Item (Tool / Weapon)
                │
                ▼
           Queries Map status
                │
                ▼
           Updates Map data only
                │
                ▼
           Map (pure data storage)
                ├── grid[y][x].type
                └── grid[y][x].hasZombie


Storage system — what needs to change and where
Remaining Issues
Two small bugs still present:
1. character.hpp — getHealth / setHealth inline stubs
cppint getHealth() {};       // returns garbage — no return statement
void setHealth(int h) {}; // silently does nothing
But character.cpp has the real implementations. The inline stubs in the header shadow the .cpp definitions — the compiler will use the empty inline versions. Fix: remove the bodies from the header, just declare:
cppint getHealth();
void setHealth(int h);
2. engine.cpp — drop (x) only works in PLAYING state but inventory is open
The x key drop is bound under GameState::PLAYING, but the player is looking at their inventory in PLAYING state (inventory opens with e but x is checked before e). This works accidentally but is fragile. The x drop logically belongs inside GameState::INVENTORY alongside f (use) and w/s (scroll) — consistent with the README spec W/S: Scroll | F: Use | X: Drop | Q: Close.


Summary of Your Damage Flow Design
Overall Flow:

Generic Event / User Action (e.g. Gun::use(), future explosive, shovel attack, etc.)

When the player decides to use an item (like firing a rifle), the game follows this sequence:

Input Phase (Engine): The Engine detects the 'F' key. It identifies which item is selected in the Player’s inventory.

Intent Phase (Item): The Engine calls item->use(). The Item checks its own internal state (Does it have ammo? Does it break?). Instead of changing the world, it returns an ActionResult (a "wish list" of what it wants to happen).

Calculation Phase (Model): The Engine takes that "wish list" (DamageRequest) and hands it to the Model. The Model looks at the Map and calculates the physics: "If a bullet starts here and goes North for 5 tiles, which coordinates does it actually hit?"

Execution Phase (Engine): The Model gives the Engine a DamageReport (a list of coordinates). The Engine then loops through those coordinates and tells the Map: tile->hasZombie = false.

Cleanup Phase (Engine): If the Item reported that it broke during use, the Engine removes it from the master list and deletes the memory.

Revised general item use system

I. Core Concept SummaryThe system operates on a Tri-Phase Loop:

Intent (The Item/Entity): The object defines what it wants to do but does not change the map itself. It returns a "wish list" of changes.

Calculation (The Model): A stateless math library that converts abstract ideas (like "Blast Radius") into concrete targets (specific memory addresses).

Execution (The Engine): The only part of the code allowed to modify memory. It processes the list of "Atomic Changes" and logs errors if an action is invalid.

II. Memory & Ownership BlueprintTo maintain a strict "XOR" relationship (an item is either on the map or in the bag), we use a Master Vault strategy:ComponentResponsibilityMemory TypeAccess RoleEngineOwns the world.std::unique_ptr<T>Master Owner: Manages the lifecycle and deletion.UserCarries items.GameEntity* (Raw)Observer: Just "holds" a reference to the master item.MapDisplays tiles/items.Tile (Unique) / Item* (Raw)Stage: Manages the grid; observes items on the ground.Change PacketTransmits intent.struct GameChangeMessenger: A temporary stack object.

III. Coding Reference Blueprint1. The Atomic Change (The Universal Language)This struct is the only thing the use() function returns.

```C++
struct GameChange {
    GameEntity* target;     // The "Who" (Direct Pointer)
    std::string property;   // The "What" (Hashed String)
    int delta;              // The "How much" (+/- value)
};
```
2. The Base Class (The Solid Interface)Every object that can be "hit" or "changed" inherits from this.
```C++
class GameEntity {
public:
    virtual void applyChange(const std::string& prop, int val) = 0;
};
```

3. The Property Hash (The Speed Booster)Inside each class, we use a switch with hashed strings to ensure $O(1)$ performance.

```C++
void Zombie::applyChange(const std::string& prop, int val) {
    switch (hash(prop.c_str())) {
        case hash("health"): 
            this->hp += val; 
            break;
        default: 
            std::cout << "[ERROR] Property not found: " << prop; 
            break;
    }
}
```
IV. Why this is the "Pro" MoveDecoupling: Your Gun doesn't need to know how a Zombie works. It just needs to know it has a "health" property.Safety: By using the Engine as the central executor, you prevent "Dangling Pointers." You only delete objects at the very end of the frame after all changes are processed.Infinite Scale:Want a Shovel? Return {TilePtr, "terrain", TRENCH_ID}.Want a Gas Mask? Return {UserPtr, "gas_resistance", 1}.Want a Trench Flare? Return {TilePtr, "visibility", 10}.V. Future Blueprint: The "Post-Action" PhaseWhen you start coding, implement in this order:Definitions: Set up the GameChange struct and GameEntity base class.The Dispatcher: Write the Engine::processChanges loop.The Model: Write the math for line-of-sight (Guns) and radius (Explosion