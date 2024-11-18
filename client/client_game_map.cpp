#include "client_game_map.h"
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


ClientGameMap::ClientGameMap(int width, int height) : width(width), height(height) {
    map.resize(height, std::vector<char>(width, EMPTY));
}


void ClientGameMap::setMap(const std::vector<std::vector<char>>& new_map) {

    // Copia los valores de new_map a map
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            map[i][j] = new_map[i][j];
        }
    }
}


int ClientGameMap::get_height(){return height;}
int ClientGameMap::get_width(){return width;}

// METODO TEMPORAL: Imprime el mapa en la consola
void ClientGameMap::printMap() const {
    for (const auto& row : map) {
        for (char cell : row) {
            std::cout << cell << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "\n";
}



std::vector<std::vector<char>> ClientGameMap::getMap(){
    return map;
}

char ClientGameMap::get_position(int x, int y) {
    char pos =  map[y][x];
    return pos;
}
