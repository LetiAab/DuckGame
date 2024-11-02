#include "game_map.h"
#include <iostream>
#include <vector>

const int EMPTY = 0;
const int DUCK = 1;
const int OBSTACLE = 2;


GameMap::GameMap(int width, int height) : width(width), height(height) {
    map.resize(height, std::vector<int>(width, EMPTY));
}

//Pone un pato en una posición (x, y) ocupando un rectángulo de 2x3
bool GameMap::placeDuck(int x, int y) {
    // me fijo si estoy dentro de los limites del mapa
    if (x + 2 > width || y + 3 > height) {
        return false;
    }
    
    // veo que no haya obstaculos
    for (int i = x; i < x + 2; ++i) {
        for (int j = y; j < y + 3; ++j) {
            if (map[j][i] == OBSTACLE) {
                return false;
            }
        }
    }
    
    // Si no hay obstáculos, marco las celdas donde esta el pato
    for (int i = x; i < x + 2; ++i) {
        for (int j = y; j < y + 3; ++j) {
            map[j][i] = DUCK;
        }
    }
    return true;
}

// METODO TEMPORAL: Imprime el mapa en la consola
void GameMap::printMap() const {
    for (const auto& row : map) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << "\n";
    }
}


