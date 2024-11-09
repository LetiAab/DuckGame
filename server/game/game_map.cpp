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


Position GameMap::move_duck_to(Position old_position, Position new_position, char duck_id) {
    int final_x = old_position.x;
    int final_y = old_position.y;

    //determino la direccion del movimiento
    int dx = (new_position.x > old_position.x) ? 1 : (new_position.x < old_position.x ? -1 : 0);
    int dy = (new_position.y > old_position.y) ? 1 : (new_position.y < old_position.y ? -1 : 0);

    //itero hasta llegar a la posición final o hasta encontrar un obstáculo
    while (final_x != new_position.x || final_y != new_position.y) {
        int next_x = final_x + dx;
        int next_y = final_y + dy;

        //verifico si la nueva posición está dentro de los límites del mapa
        if (next_x < 0 || next_x + 1 >= width || next_y < 0 || next_y + 2 >= height) {
            break;
        }

        // compruebo que este libre
        bool is_free = true;
        for (int y = next_y; y < next_y + 3; ++y) {
            for (int x = next_x; x < next_x + 2; ++x) {
                if (map[y][x] != EMPTY && map[y][x] != duck_id) {
                    is_free = false;
                    break;
                }
            }
            if (!is_free) break;
        }

        //si el área está libre, actualizo la posición final
        if (is_free) {
            final_x = next_x;
            final_y = next_y;
        } else {
            //si hay un obstáculo, me quedo en la última posición libre
            break;
        }
    }

    //actualizo la posición del pato en el mapa
    cleanDuckOldPosition(old_position.x, old_position.y);
    setDuckNewPosition(final_x, final_y, duck_id);

    return Position(final_x, final_y);
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

    // Debug platforms
    map[height - 2][3] = PLATFORM;
    map[height - 2][4] = PLATFORM;

    map[height - 2][7] = PLATFORM;
    map[height - 2][8] = PLATFORM;
    map[height - 3][7] = PLATFORM;
    map[height - 3][8] = PLATFORM;

    map[height - 2][11] = PLATFORM;
    map[height - 2][12] = PLATFORM;

    //plataforma en el aire
    map[height - 6][16] = PLATFORM;
    map[height - 6][17] = PLATFORM;
    map[height - 6][18] = PLATFORM;

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
