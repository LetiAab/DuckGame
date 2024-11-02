#include "game_map.h"
#include <iostream>
#include <vector>

const int EMPTY = 0;
const int DUCK = 1;
const int OBSTACLE = 2;


GameMap::GameMap(int width, int height) : width(width), height(height) {
    map.resize(height, std::vector<int>(width, EMPTY));
}

//Esto no contempla colisiones entre patos
bool GameMap::canMoveDuckTo(int x, int y) {

    // Verifica que las coordenadas estén dentro de los límites del mapa
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false; 
    }

    for (int i = x; i < x + 2; ++i) {
        for (int j = y; j < y + 3; ++j) {
            if (map[j][i] == OBSTACLE) {
                return false;
            }
        }
    }
    
    return true;
}

void GameMap::setEscenario() {

    for (int x = 0; x < width; ++x) {
        map[height - 1][x] = OBSTACLE; // Piso
    }
    
    // Coloca obstáculos en los costados (x = 0 y x = width - 1)
    for (int y = 0; y < height; ++y) {
        map[y][0] = OBSTACLE;          // Costado izquierdo
        map[y][width - 1] = OBSTACLE;  // Costado derecho
    }
}


void GameMap::cleanDuckOldPosition(int x, int y) {
    for (int i = x; i < x + 2; ++i) { 
        for (int j = y; j < y + 3; ++j) {
            if (j >= 0 && j < height && i >= 0 && i < width) { // Verificar límites
                map[j][i] = EMPTY; 
            }
        }
    }
}

void GameMap::setDuckNewPosition(int x, int y) {
    for (int i = x; i < x + 2; ++i) {
        for (int j = y; j < y + 3; ++j) {
            if (j >= 0 && j < height && i >= 0 && i < width) { // Verificar límites
                map[j][i] = DUCK; 
            }
        }
    }
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

int GameMap::get_height(){return height;}
int GameMap::get_width(){return width;}

// METODO TEMPORAL: Imprime el mapa en la consola
void GameMap::printMap() const {
    for (const auto& row : map) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "\n";

}



