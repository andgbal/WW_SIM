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