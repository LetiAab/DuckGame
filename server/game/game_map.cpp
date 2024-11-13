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

const char VOID = 'V'; //VACIO


const char BULLET = '*';


GameMap::GameMap(int width, int height) : width(width), height(height) {
    map.resize(height, std::vector<char>(width, EMPTY));
}

bool GameMap::canMoveBulletTo(int x, int y, char duck_id){
    
    //printMap();

    
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false; 
    }
    

    if (map[y][x] == duck_id) {
        std::cout << "CHOCO CONMIGO MISMO" << "\n";
        return true;
    }


    if(map[y][x] != EMPTY){
        
        //bala choca con ALGO
        return false;
    }
    

    return true;

}

void GameMap::setBulletNewPosition(int x, int y){

    map[y][x] = BULLET;
}

void GameMap::cleanBulletOldPosition(int x, int y){

    map[y][x] = EMPTY;

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
        if (next_x < 0 || next_x + DUCK_SIZE_X-1 >= width || next_y < 0 || next_y + DUCK_SIZE_Y-1 >= height) {
            break;
        }

        // compruebo que este libre
        bool is_free = true;
        for (int y = next_y; y < next_y + DUCK_SIZE_Y; ++y) {
            for (int x = next_x; x < next_x + DUCK_SIZE_X; ++x) {
                if (map[y][x] != EMPTY && map[y][x] != duck_id) {
                    is_free = false;
                    break;
                }
            }
            if (!is_free) break;
        }

        if (next_x == new_position.x){
            dx = 0;
        }

        if (next_y == new_position.y){
            dy = 0;
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

bool GameMap::duckIsOverVoid(int x, int y) {
    std::cout << "REVISO SI ESTOY SOBRE EL VACIO" << "\n";

    std::cout << "Valores: x = " << x << ", y = " << y 
              << ", width = " << width << ", height = " << height << std::endl;

    // Verifica si el pato esta sobre el vacio
    if (map[y + DUCK_SIZE_Y][x] == VOID) {
        std::cout << "ESTOY SOBRE EL VACIO" << "\n";
        return true;
    }

    return false;
}

bool GameMap::duckIsOverBullet(Position position) {
    std::cout << "REVISO SI ME PEGO UNA BALA" << "\n";

    // Verifica si el pato fue golpeado por una bala
    for (int i = position.x; i < position.x + DUCK_SIZE_X; ++i) {
        for (int j = position.y; j < position.y + DUCK_SIZE_Y; ++j) {
            if (map[j][i] == BULLET) {
                std::cout << "ME DIERON!" << "\n";
                return true;
            }
        }
    }

    return false;
}

bool GameMap::canMoveDuckTo(int x, int y, char duck_id) {

    // Verifica que las coordenadas estén dentro de los límites del mapa
     if (x < 0 || x + DUCK_SIZE_X > width || y < 0 || y + DUCK_SIZE_Y > height) {
        return false;
    }

    // Verifica que el movimiento no colisione con una plataforma
    for (int i = x; i < x + DUCK_SIZE_X; ++i) {
        for (int j = y; j < y + DUCK_SIZE_Y; ++j) {
            if (map[j][i] == PLATFORM) {
                return false;
            }
        }
    }

    // Verifica que el movimiento no colisione con el vacio, porque lo borraria
    for (int i = x; i < x + DUCK_SIZE_X; ++i) {
        for (int j = y; j < y + DUCK_SIZE_Y; ++j) {
            if (map[j][i] == VOID) {
                return false;
            }
        }
    }

    // Verifica que el movimiento no colisione con otro pato
    for (int i = x; i < x + DUCK_SIZE_X; ++i) {
        for (int j = y; j < y + DUCK_SIZE_Y; ++j) {
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

char GameMap::at(Position position) {
    return map[position.y][position.x];
}


bool GameMap::is_duck_touching_floor(int x, int y) {
    // Verifica que el pato no tenga plataformas en el piso
    int j = y + DUCK_SIZE_Y;
    for (int i = x; i < x + DUCK_SIZE_X; ++i) {
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
    for (int i = x; i < x + DUCK_SIZE_X; ++i) {
        for (int j = y; j < y + DUCK_SIZE_Y; ++j) {
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
    //printMap();
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
    // SETEO EL VACIO. SI COLISIONO CON EL VACIO MUERO
    for (int x = 0; x < width; ++x) {
        map[height - 1][x] = VOID;
    }

    // Piso principal
    for (int x = 15; x < width - 15; ++x) {
        map[height - 10][x] = PLATFORM;
        map[height - 11][x] = PLATFORM;
        map[height - 12][x] = PLATFORM;
    }

    // Costados (barreras izquierda y derecha)
    for (int y = 0; y < height; ++y) {
        map[y][0] = PLATFORM;
        map[y][width - 1] = PLATFORM;
    }

    // Plataformas adicionales de diferentes tamaños:
    
    for (int y = height - 100; y < height - 120; ++y) {
        for (int x = 20; x < 25; ++x) {
            map[y][x] = PLATFORM;
        }
    }

    for (int y = height - 100; y < height - 105; ++y) {
        for (int x = 100; x < 130; ++x) {
            map[y][x] = PLATFORM;
        }
    }


    for (int y = height - 30; y < height - 25; ++y) {
        for (int x = width - 60; x < width - 50; ++x) {
            map[y][x] = PLATFORM;
        }
    }

    for (int y = height - 60; y < height - 55; ++y) {
        for (int x = width - 100; x < width - 80; ++x) {
            map[y][x] = PLATFORM;
        }
    }

}


void GameMap::cleanDuckOldPosition(int x, int y) {
    for (int i = x; i < x + DUCK_SIZE_X; ++i) { 
        for (int j = y; j < y + DUCK_SIZE_Y; ++j) {
            if (j >= 0 && j < height && i >= 0 && i < width) { // Verificar límites
                map[j][i] = EMPTY; 
            }
        }
    }
}

void GameMap::setDuckNewPosition(int x, int y, char duck_id) {
    for (int i = x; i < x + DUCK_SIZE_X; ++i) {
        for (int j = y; j < y + DUCK_SIZE_Y; ++j) {
            if (j >= 0 && j < height && i >= 0 && i < width) { // Verificar límites
                map[j][i] = duck_id; 
            }
        }
    }
}




//Pone un pato en una posición (x, y) ocupando un rectángulo de DUCK_SIZE_X x DUCK_SIZE_Y
bool GameMap::placeDuck(int x, int y, char duck_id) {


    if (x < 0 || y < 0 || (x + DUCK_SIZE_X) > width || (y + DUCK_SIZE_Y) > height) {
        return false;
    }
    
    if(duckIsOverVoid(x,y)){
        return false;
    }

    // veo que no haya obstaculos
    for (int i = x; i < x + DUCK_SIZE_X; ++i) {
        for (int j = y; j < y + DUCK_SIZE_Y; ++j) {
            if (map[j][i] == PLATFORM) {
                return false;
            }
        }
    }

    // veo que no haya otro pato
    for (int i = x; i < x + DUCK_SIZE_X; ++i) {
        for (int j = y; j < y + DUCK_SIZE_Y; ++j) {
            if ((map[j][i] == DUCK_1) || (map[j][i] == DUCK_2) || (map[j][i] == DUCK_3)
             || (map[j][i] == DUCK_4) || (map[j][i] == DUCK_5) || (map[j][i] == DUCK_6)) {
                return false;
            }
        }
    }
    
    // Si no hay obstáculos, marco las celdas donde esta el pato
    for (int i = x; i < x + DUCK_SIZE_X; ++i) {
        for (int j = y; j < y + DUCK_SIZE_Y; ++j) {
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
