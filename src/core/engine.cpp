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
        else if (in == 'x'){
            Item* dropped = user.removeFromInventory(selectedItemIdx);
            map.placeItem(user.pos, dropped);


                if (selectedItemIdx >= (int)user.storage.count())
                    selectedItemIdx = (int)user.storage.count() - 1;
                if (selectedItemIdx < 0) selectedItemIdx = 0;
        }
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