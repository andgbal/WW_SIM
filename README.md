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
→ Calls a centralized model function, passing: Position, damage, accuracy, range, and damage type (line / tile).
Model Layer handles the pure calculation:
Line Damage: Starts from source position, travels in a direction (usually player's facing), applies damage along the line, stops at first entity hit.
Tile Damage: Applies damage to a fixed shape/area of tiles around or in front of the source position (e.g. cone, circle, rectangle).

Return Value: A struct containing a std::vector of hit positions + damage information.
Engine receives the result and:
Updates the Map (clear hasZombie, etc.).
Triggers follow-up events (zombie death, sound, visual feedback, chain reactions).


This is a solid evolution from the current line-only shooting in Gun::use().
Comments on the Design