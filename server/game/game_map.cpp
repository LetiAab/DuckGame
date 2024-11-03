#include "game_map.h"
#include <iostream>
#include <vector>

const char EMPTY = ' ';
const char DUCK_1 = '1';
const char DUCK_2 = '2';
const char DUCK_3 = '3';
const char DUCK_4 = '4';
const char DUCK_5 = '5';
const char DUCK_6 = '6';
const char PLATFORM = 'P';


GameMap::GameMap(int width, int height) : width(width), height(height) {
    map.resize(height, std::vector<char>(width, EMPTY));
}

bool GameMap::canMoveDuckTo(int x, int y, char duck_id) {

    // Verifica que las coordenadas estén dentro de los límites del mapa
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false; 
    }

    // Verifica que el movimiento no colisione con una plataforma
    for (int i = x; i < x + 2; ++i) {
        for (int j = y; j < y + 3; ++j) {
            if (map[j][i] == PLATFORM) {
                return false;
            }
        }
    }

    // Verifica que el movimiento no colisione con otro pato
    for (int i = x; i < x + 2; ++i) {
        for (int j = y; j < y + 3; ++j) {
            for (int l = 1; l < 7; l++) {
                char duck_number = static_cast<char>(l + '0');
                if (duck_number == duck_id) {
                    continue;
                }
                if (map[j][i] == duck_number) {
                    return false;
                }
            }
        }
    }
    
    return true;
}

void GameMap::setEscenario() {

    for (int x = 0; x < width; ++x) {
        map[height - 1][x] = PLATFORM; // Piso
    }
    
    // Coloca obstáculos en los costados (x = 0 y x = width - 1)
    for (int y = 0; y < height; ++y) {
        map[y][0] = PLATFORM;          // Costado izquierdo
        map[y][width - 1] = PLATFORM;  // Costado derecho
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

void GameMap::setDuckNewPosition(int x, int y, char duck_id) {
    for (int i = x; i < x + 2; ++i) {
        for (int j = y; j < y + 3; ++j) {
            if (j >= 0 && j < height && i >= 0 && i < width) { // Verificar límites
                map[j][i] = duck_id; 
            }
        }
    }
}


//Pone un pato en una posición (x, y) ocupando un rectángulo de 2x3
bool GameMap::placeDuck(int x, int y, char duck_id) {
    // me fijo si estoy dentro de los limites del mapa
    if (x + 2 > width || y + 3 > height) {
        return false;
    }
    std::cout << "El unsigned char es " << duck_id << std::endl;
    
    // veo que no haya obstaculos
    for (int i = x; i < x + 2; ++i) {
        for (int j = y; j < y + 3; ++j) {
            if (map[j][i] == PLATFORM) {
                return false;
            }
        }
    }

    // veo que no haya otro pato
    for (int i = x; i < x + 2; ++i) {
        for (int j = y; j < y + 3; ++j) {
            if ((map[j][i] == DUCK_1) || (map[j][i] == DUCK_2) || (map[j][i] == DUCK_3)
             || (map[j][i] == DUCK_4) || (map[j][i] == DUCK_5) || (map[j][i] == DUCK_6)) {
                return false;
            }
        }
    }
    
    // Si no hay obstáculos, marco las celdas donde esta el pato
    for (int i = x; i < x + 2; ++i) {
        for (int j = y; j < y + 3; ++j) {
            map[j][i] = duck_id;
        }
    }
    return true;
}

int GameMap::get_height(){return height;}
int GameMap::get_width(){return width;}

// METODO TEMPORAL: Imprime el mapa en la consola
void GameMap::printMap() const {
    for (const auto& row : map) {
        for (char cell : row) {
            std::cout << cell << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "\n";
}



std::vector<std::vector<char>> GameMap::getMap(){
    return map;
}
