#include "engine.hpp"
#include "../items/tools.hpp"
#include "../items/weapons.hpp"
#include <iostream>
#include <conio.h>   // For _getch() and _kbhit()
#include <windows.h> // For Sleep() and system("cls")

Engine::Engine() : player(25, 25), state(GameState::PLAYING), selectedItemIdx(0), running(true) {
    // Initial Setup
    player.inventory.push_back(new Shovel());
    player.inventory.push_back(new Rifle());
    
    // Spawn random zombies
    for (int i = 0; i < 30; i++) {
        int zx = rand() % map.width;
        int zy = rand() % map.height;
        if (zx != 25 || zy != 25) { // Don't spawn on player
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
    // Moves cursor to top-left instead of full clear to reduce flicker
    COORD cursorPosition; cursorPosition.X = 0; cursorPosition.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);

    std::cout << "--- TRENCH ZOMBIES | Action Economy System ---\n";
    
    if (state == GameState::INVENTORY) {
        std::cout << "\n  [ BACKPACK ]\n  W/S: Scroll | F: Use | Q: Close\n  " << std::string(30, '-') << "\n";
        for (int i = 0; i < (int)player.inventory.size(); i++) {
            std::cout << (i == selectedItemIdx ? " > " : "   ") << player.inventory[i]->name << "\n";
        }
        // Fill space to prevent ghosting from map
        for(int i=0; i<10; i++) std::cout << std::string(40, ' ') << "\n";
        return;
    }

    std::cout << "Facing: " << (int)player.facing << " | Move: WASD | Turn: IJKL | Menu: E\n";
    
    int viewH = 7;  // Vertical radius
    int viewW = 15; // Horizontal radius

    for (int y = player.pos.y - viewH; y <= player.pos.y + viewH; y++) {
        for (int x = player.pos.x - viewW; x <= player.pos.x + viewW; x++) {
            // Draw Player
            if (x == player.pos.x && y == player.pos.y) {
                char pSym = (player.facing == Dir::NORTH) ? '^' : 
                            (player.facing == Dir::SOUTH) ? 'v' : 
                            (player.facing == Dir::EAST)  ? '>' : '<';
                std::cout << pSym << " ";
            } 
            // Draw Out of Bounds
            else if (x < 0 || x >= map.width || y < 0 || y >= map.height) {
                std::cout << "X ";
            }
            // Draw Visible Area
            else if (map.inVisionCone(player.pos, player.facing, x, y, 8)) {
                std::cout << map.getTile(x, y)->getSymbol() << " ";
            } 
            // Draw Darkness
            else {
                std::cout << "  ";
            }
        }
        std::cout << "\n";
    }
}

void Engine::handleInput() {
    if (!_kbhit()) return;

    int in = _getch();
    bool tookAction = false;

    if (state == GameState::PLAYING) {
        // Move
        if (in == 'w' && player.pos.y > 0) { player.pos.y--; tookAction = true; }
        else if (in == 's' && player.pos.y < map.height - 1) { player.pos.y++; tookAction = true; }
        else if (in == 'a' && player.pos.x > 0) { player.pos.x--; tookAction = true; }
        else if (in == 'd' && player.pos.x < map.width - 1) { player.pos.x++; tookAction = true; }
        // Aim/Turn
        else if (in == 'i') { player.facing = Dir::NORTH; tookAction = true; }
        else if (in == 'k') { player.facing = Dir::SOUTH; tookAction = true; }
        else if (in == 'j') { player.facing = Dir::WEST;  tookAction = true; }
        else if (in == 'l') { player.facing = Dir::EAST;  tookAction = true; }
        // Menu
        else if (in == 'e') { system("cls"); state = GameState::INVENTORY; }
        else if (in == 'q') running = false;
    } 
    else if (state == GameState::INVENTORY) {
        if (in == 'w' && selectedItemIdx > 0) selectedItemIdx--;
        else if (in == 's' && selectedItemIdx < (int)player.inventory.size() - 1) selectedItemIdx++;
        else if (in == 'q') { system("cls"); state = GameState::PLAYING; }
        else if (in == 'f' || in == 13) {
            player.inventory[selectedItemIdx]->use(player, map);
            system("cls");
            state = GameState::PLAYING;
            tookAction = true;
        }
    }

    if (tookAction) tick();
}

void Engine::run() {
    system("cls"); // Initial clear
    while (running) {
        render();
        handleInput();
        Sleep(30); // Capped at ~33 FPS
    }
}