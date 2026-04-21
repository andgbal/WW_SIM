#include "engine.hpp"
#include "../items/tools.hpp"
#include "../items/weapons.hpp"
#include <iostream>
#include <windows.h>

Engine::Engine() : player(25, 25), state(GameState::PLAYING), selectedItemIdx(0), running(true) {
    // Hide cursor for cleanliness
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    player.inventory.push_back(new Shovel());
    player.inventory.push_back(new Rifle());
    
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
        z.update(map, player.pos);
    }
}

void Engine::render() {
    system("cls"); 

    std::cout << "--- TRENCH ZOMBIES | STABLE VERSION ---\n";
    
    if (state == GameState::INVENTORY) {
        std::cout << "\n  [ BACKPACK ]\n  W/S: Scroll | F: Use | Q: Close\n";
        std::cout << "  ------------------------------\n";
        for (int i = 0; i < (int)player.inventory.size(); i++) {
            std::cout << (i == selectedItemIdx ? " > " : "   ") << player.inventory[i]->name << "\n";
        }
        return;
    }

    std::cout << "Controls: WASD (Move) | IJKL (Turn) | E (Menu)\n";
    std::cout << "Current Action: Waiting for input...\n\n";
    
    // Camera Viewport Settings
    int viewH = 6; 
    int viewW = 12;

    for (int y = player.pos.y - viewH; y <= player.pos.y + viewH; y++) {
        for (int x = player.pos.x - viewW; x <= player.pos.x + viewW; x++) {
            
            // 1. Draw Player (Centered)
            if (x == player.pos.x && y == player.pos.y) {
                if(player.facing == Dir::NORTH)      std::cout << "^ ";
                else if(player.facing == Dir::SOUTH) std::cout << "v ";
                else if(player.facing == Dir::EAST)  std::cout << "> ";
                else                                 std::cout << "< ";
            } 
            // 2. Draw Boundaries
            else if (x < 0 || x >= map.width || y < 0 || y >= map.height) {
                std::cout << "X "; 
            }
            // 3. Draw Vision Cone (Light)
            else if (map.inVisionCone(player.pos, player.facing, x, y, 8)) {
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
        if (in == 'w' && player.pos.y > 0) { player.pos.y--; tookAction = true; }
        else if (in == 's' && player.pos.y < map.height - 1) { player.pos.y++; tookAction = true; }
        else if (in == 'a' && player.pos.x > 0) { player.pos.x--; tookAction = true; }
        else if (in == 'd' && player.pos.x < map.width - 1) { player.pos.x++; tookAction = true; }
        else if (in == 'i') { player.facing = Dir::NORTH; tookAction = true; }
        else if (in == 'k') { player.facing = Dir::SOUTH; tookAction = true; }
        else if (in == 'j') { player.facing = Dir::WEST;  tookAction = true; }
        else if (in == 'l') { player.facing = Dir::EAST;  tookAction = true; }
        else if (in == 'e') state = GameState::INVENTORY;
        else if (in == 'q') running = false;
    } 
    else if (state == GameState::INVENTORY) {
        if (in == 'w' && selectedItemIdx > 0) selectedItemIdx--;
        else if (in == 's' && selectedItemIdx < (int)player.inventory.size() - 1) selectedItemIdx++;
        else if (in == 'q') state = GameState::PLAYING;
        else if (in == 'f') {
            player.inventory[selectedItemIdx]->use(player, map);
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