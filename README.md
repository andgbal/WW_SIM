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
1. Inventory class — new file, replaces the raw vector
Right now player.inventory is just std::vector<Item*>. Make it a proper class in character.hpp (or its own inventory.hpp). It should own:
cppclass Inventory {
public:
    int maxWeight;
    int currentWeight;
    std::vector<Item*> slots;

    bool add(Item* item);   // returns false if over weight limit
    void drop(int idx);     // removes from slots, deletes pointer
    Item* get(int idx);
    int count() const;
};
Player then holds Inventory storage instead of std::vector<Item*> inventory.

2. Item base class — needs a weight field
Every item needs a weight so the inventory can enforce a limit. Add to item.hpp:
cppint weight;  // set by each subclass constructor
Each subclass constructor passes its weight up: Item("Shovel", 3), Item("Rifle", 5), Item("HealthKit", 1).

3. Player — add a pickup(Item*) method
The player shouldn't reach into their own inventory directly from the engine. Add:
cppbool pickup(Item* item);   // delegates to storage.add(), checks weight
void dropItem(int idx);    // delegates to storage.drop()
This keeps the engine's job simple: detect item on tile → call player.pickup(item).

4. Engine / handleInput — add drop + pickup triggers
Currently INVENTORY state only has scroll (w/s), use (f), and close (q). Add:

x key → player.dropItem(selectedItemIdx) — removes from inventory, places Item* back on current map tile (needs Tile to hold an Item* first, see item 5)
Pickup happens automatically in tick() when player steps onto a tile with a ground item — not a manual keypress


5. Tile — needs a ground item slot
Tile currently has type and hasZombie. Add:
cppItem* groundItem = nullptr;  // null means tile is empty
Map needs two helpers:
cppvoid placeItem(int x, int y, Item* item);
Item* takeItem(int x, int y);   // returns pointer and sets tile back to nullptr

6. Engine::tick() — check for pickup after every player move
After player moves, check the new tile:
cppItem* found = map.takeItem(player.pos.x, player.pos.y);
if (found) {
    if (!player.pickup(found)) {
        map.placeItem(player.pos.x, player.pos.y, found); // too heavy, put back
    }
}

7. render() — inventory display needs updating
The backpack screen currently shows just item->name. With the new system it should also show weight per item and total carried vs max:
[ BACKPACK ]  7 / 10 kg
W/S: Scroll | F: Use | X: Drop | Q: Close
------------------------------
 > Shovel       3kg
   Rifle        5kg
   HealthKit    1kg  [x2]   ← stretch goal: stackable consumables

Summary of files touched
FileWhat changesitem.hppadd weight field to base classtools.hpp/.cpppass weight in each constructorweapons.hpp/.cpppass weight in each constructorcharacter.hppreplace vector<Item*> with Inventory storage; add pickup(), dropItem()character.cppimplement pickup(), dropItem()terrain.hppadd Item* groundItem to Tile; add placeItem/takeItem to Mapterrain.cppimplement placeItem, takeItemengine.cppupdate tick() for auto-pickup; update handleInput() for drop key; update render() inventory display