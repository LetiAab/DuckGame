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

bool GameMap::is_duck_touching_floor(int x, int y) {
    // Verifica que el pato no tenga plataformas en el piso
    int j = y + 3;
    for (int i = x; i < x + 2; ++i) {
        if (map[j][i] == PLATFORM) {
            return true;
        }
    }
    
    return false;
}

bool GameMap::is_element_touching_floor(int x, int y, int size_x, int size_y) {
    // Esto supone un tamaño de la banana de 1x1
    int j = y + size_y;
    for (int i = x; i < x + size_x; ++i) {
        if (map[j][i] == PLATFORM) {
            return true;
        }
    }
    
    return false;
}

// Falta usar el size, estás usando los del pato
bool GameMap::can_move_projectile(int x, int y, int size_x, int size_y) {

    // Verifica que las coordenadas estén dentro de los límites del mapa
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false; 
    }

    // Verifica que el movimiento no colisione con una plataforma
    for (int i = x; i < x + size_x; ++i) {
        for (int j = y; j < y + size_y; ++j) {
            if (map[j][i] == PLATFORM) {
                return false;
            }
        }
    }
    
    return true;
}

// Falta usar el size, estás usando los del pato
int GameMap::projectile_hits_duck(int x, int y) {
    // Verifica si colisiona con un pato
    for (int i = x; i < x + 2; ++i) {
        for (int j = y; j < y + 3; ++j) {
            for (int l = 1; l < 7; l++) {
                char duck_number = static_cast<char>(l + '0');
                if (map[j][i] == duck_number) {
                    return duck_number;
                }
            }
        }
    }

    return 0;
}

void GameMap::clean_projectile_old_position(int x, int y, int size_x, int size_y) {
    for (int i = x; i < x + size_x; ++i) { 
        for (int j = y; j < y + size_y; ++j) {
            if (j >= 0 && j < height && i >= 0 && i < width) { // Verificar límites
                map[j][i] = EMPTY; 
            }
        }
    }
}

void GameMap::set_projectile_new_position(int x, int y,  int size_x, int size_y) {
    for (int i = x; i < x + size_x; ++i) {
        for (int j = y; j < y + size_y; ++j) {
            if (j >= 0 && j < height && i >= 0 && i < width) {
                map[j][i] = 'P'; // Cambiar, deberías recibir la letra asignada al proyectil
            }
        }
    }
}

void GameMap::move_projectile(int position_x, int position_y, int speed_x, int speed_y, int size_x, int size_y) {
    // Gravity check. Can be modularized
    if (can_move_projectile(position_x, position_y + 1, size_x, size_y)) { // check if use position_x or delta_x
        clean_projectile_old_position(position_x, position_y, size_x, size_y);

        position_y += 1;

        set_projectile_new_position(position_x, position_y, size_x, size_y);
    }

    int delta_x = position_x + speed_x;
    int delta_y = position_y + speed_y;

    if (can_move_projectile(delta_x, delta_y, size_x, size_y)) {
        clean_projectile_old_position(position_x, position_y, size_x, size_y);

        position_x = delta_x;
        position_y = delta_y;

        set_projectile_new_position(delta_x, delta_y, size_x, size_y);
    }
    printMap();
}

char GameMap::duck_in_position(int x, int y, int size_x, int size_y) {
    // Verifica que haya un pato, si no hay devuelve 0
    for (int i = x; i < x + size_x; ++i) {
        for (int j = y; j < y + size_y; ++j) {
            for (int l = 1; l < 7; l++) {
                char duck_number = static_cast<char>(l + '0');
                if (map[j][i] == duck_number) {
                    return duck_number;
                }
            }
        }
    }
    return 0;
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

    // Debug platforms
    map[height - 2][3] = PLATFORM;
    map[height - 2][4] = PLATFORM;

    map[height - 2][7] = PLATFORM;
    map[height - 2][8] = PLATFORM;
    map[height - 3][7] = PLATFORM;
    map[height - 3][8] = PLATFORM;

    map[height - 2][11] = PLATFORM;
    map[height - 2][12] = PLATFORM;
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
    if (x < 0 || y < 0 || (x + 2) > width || (y + 3) > height) {
        return false;
    }
    
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

char GameMap::get_position(int x, int y) {
    char pos =  map[y][x];
    return pos;
}
