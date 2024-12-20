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
const char ITEM = 'I';

const char VOID = 'V';

const char BOX = 'B';
const char BANANA = 'b';
const char BULLET = '*';


GameMap::GameMap(int width, int height) : width(width), height(height) {
    map.resize(height, std::vector<char>(width, EMPTY));
}


void GameMap::setBulletNewPosition(Position pos) {
    for (int y = pos.y; y < pos.y + BULLET_SIZE_Y; ++y) {
        for (int x = pos.x; x < pos.x + BULLET_SIZE_X; ++x) {
            map[y][x] = BULLET;
        }
    }
}

void GameMap::cleanBulletOldPosition(Position pos) {
    for (int y = pos.y; y < pos.y + BULLET_SIZE_Y; ++y) {
        for (int x = pos.x; x < pos.x + BULLET_SIZE_X; ++x) {
            map[y][x] = EMPTY;
        }
    }
}

bool GameMap::bullet_hit_other_duck(char e, char duck_shooting){

    return (((e == DUCK_1) || (e == DUCK_2) || (e == DUCK_3) || (e == DUCK_4) ||
    (e == DUCK_5) || (e == DUCK_6)) && (e != duck_shooting));
}

Position GameMap::try_move_bullet_to(Position old_position, Position new_position, char duck_id, bool& hit_something){
    int final_x = old_position.x;
    int final_y = old_position.y;

    //determino la direccion del movimiento
    int dx = (new_position.x > old_position.x) ? 1 : (new_position.x < old_position.x ? -1 : 0);
    int dy = (new_position.y > old_position.y) ? 1 : (new_position.y < old_position.y ? -1 : 0);
    

    //itero hasta llegar a la posición final, encontrar un obstaculo o pegarle a un pato
    while (final_x != new_position.x || final_y != new_position.y) {
        //std::cout << "dx es " << dx << " y dy es " << dy << std::endl;
        
        int next_x = final_x + dx;
        int next_y = final_y + dy;

        //verifico si la nueva posición está dentro de los límites del mapa
        if (next_x < 0 || next_x + BULLET_SIZE_X > width || next_y < 0 || next_y + BULLET_SIZE_Y > height) {
            break;
        }

        
        for (int y = next_y; y < next_y + BULLET_SIZE_Y; ++y) {
            for (int x = next_x; x < next_x + BULLET_SIZE_X; ++x) {

                if (map[y][x] == PLATFORM) {
                    // Caso 1: choque con una plataforma
                    hit_something = true;
                    return Position(final_x, final_y);  // devuelvo la posición actual
                }
                else if (map[y][x] == BOX) {
                    std::cout << "bala choca con una caja \n";

                    hit_something = true;
                    // avanzo una posición más para que la bala quede "dentro" de la box
                    final_x = next_x;
                    final_y = next_y;
                    return Position(final_x, final_y);
                }
                else if (bullet_hit_other_duck(map[y][x], duck_id)) {
                    // Caso 2: choque con otro pato
                    hit_something = true;
                    // avanzo una posición más para que la bala quede "dentro" del pato
                    final_x = next_x;
                    final_y = next_y;
                    return Position(final_x, final_y);
                }
            }
        }

        if (next_x == new_position.x){
            dx = 0;
        }

        if (next_y == new_position.y){
            dy = 0;
        }

        //si el área está libre, actualizo la posición final
        final_x = next_x;
        final_y = next_y;
        
    }

    return Position(final_x, final_y);

}

Position GameMap::try_move_bouncing_laser_to(Position old_position, Position new_position, char duck_id, bool& hit_something, bool& hit_platform, bool& hit_x){
    int final_x = old_position.x;
    int final_y = old_position.y;

    //determino la direccion del movimiento
    int dx = (new_position.x > old_position.x) ? 1 : (new_position.x < old_position.x ? -1 : 0);
    int dy = (new_position.y > old_position.y) ? 1 : (new_position.y < old_position.y ? -1 : 0);

    std::cout << "TryMoveBouncingLaserTo. Comienzo trayectoria desde x: " << old_position.x << " y: " << old_position.y << "\n";

    //itero hasta llegar a la posición final, encontrar un obstaculo o pegarle a un pato
    while (final_x != new_position.x || final_y != new_position.y) {
        int next_x = final_x + dx;
        int next_y = final_y + dy;

        std::cout << "La posición tentativa de esta iteración es x: " << next_x << " y: " << next_y << "\n";


        //verifico si la nueva posición está dentro de los límites del mapa
        if (next_x < 0 || next_x + BULLET_SIZE_X > width || next_y < 0 || next_y + BULLET_SIZE_Y > height) {
            break;
        }

        
        for (int y = next_y; y < next_y + BULLET_SIZE_Y; ++y) {
            for (int x = next_x; x < next_x + BULLET_SIZE_X; ++x) {

                if (map[y][x] == PLATFORM) {
                    std::cout << "Choco con una plataforma \n";

                    // Nota: El proximo upgrade que debería tener esto es que el movimiento no se termine cuando
                    // choca con una pared, sino que recalcule el esperado
                    hit_platform = true;

                    // Si el choque fue en una coordenada, espejo la velocidad en esa coordenada y la otra la mantengo igual
                    // Si cae en un zócalo morimos todos (?, o espejo las dos

                    // Notar que tengo que recalcular la posición esperada. Supongo que puedo hacer una diff 
                    // con new_position y poner esa diff pero invertida

                    // El choque es en x
                    if (map[y][x-dx] != PLATFORM) {
                        hit_x = true;                        
                        // Aumento la coordenada no espejada (decisión de diseño)
                        //y += dy;

                        // invierto la coordenada espejada
                        //dx = 0 - dx;

                        // recalculo la nueva posición

                        //return Position(final_x, final_y);  // devuelvo la posición actual
                    } else {
                        hit_x = false;
                    }
                    return Position(final_x, final_y);  // devuelvo la posición actual
                }
                else if (map[y][x] == BOX) {
                    std::cout << "el laser choca con una caja \n";

                   hit_something = true;

                   final_x = next_x;
                   final_y = next_y;
                   return Position(final_x, final_y);
                }
                else if (bullet_hit_other_duck(map[y][x], duck_id)) {
                    std::cout << "Choco con un pato (no el owner) \n";
                    // Caso 2: choque con otro pato
                    hit_something = true;
                    // avanzo una posición más para que la bala quede "dentro" del pato
                    final_x = next_x;
                    final_y = next_y;
                    return Position(final_x, final_y);
                }
            }
        }

        if (next_x == new_position.x){
            dx = 0;
        }

        if (next_y == new_position.y){
            dy = 0;
        }

        //si el área está libre, actualizo la posición final
        final_x = next_x;
        final_y = next_y;

        std::cout << "Se alcanzó la posición tentativa, x: " << final_x << " y: " << final_y << "\n";
    }

    return Position(final_x, final_y);

}

bool GameMap::is_throwable_touching_floor(Position position, int size_x, int size_y) {
    // Verifica que el pato no tenga plataformas en el piso
    int j = position.y + size_y;
    for (int i = position.x; i < position.x + size_x; ++i) {
        if (map[j][i] == PLATFORM) {
            return true;
        }
    }
    
    return false;
}

// La idea es, muevo a la granada como parabola, y después si cae al piso ya no se mueve
Position GameMap::try_move_grenade(Position old_position, Position speed, bool& hit_void) {
    // Si está tocando el piso ya no se mueve más
    if (is_throwable_touching_floor(old_position, 1, 1)) {
        return old_position;
    }
    int final_x = old_position.x;
    int final_y = old_position.y;

    //determino la direccion del movimiento
    int dx = (speed.x > 0) ? 1 : ((speed.x < 0) ? -1 : 0);
    int dy = (speed.y > 0) ? 1 : ((speed.y < 0) ? -1 : 0);

    Position new_position(old_position.x + speed.x, old_position.y + speed.y);

    //itero hasta llegar a la posición final, encontrar un obstaculo o pegarle a un pato
    while (final_x != new_position.x || final_y != new_position.y) {
        //std::cout << "dx es " << dx << " y dy es " << dy << std::endl;
        
        int next_x = final_x + dx;
        int next_y = final_y + dy;

        //verifico si la nueva posición está dentro de los límites del mapa
        if (next_x < 0 || next_x + 1 > width || next_y < 0 || next_y + 1 > height) {
            break;
        }

        
        for (int y = next_y; y < next_y + 1; ++y) {
            for (int x = next_x; x < next_x + 1; ++x) {

                std::cout << "avancé la granada, esta en la pos x: " << x << " y: " << y << ", donde hay un " << map[y][x] << std::endl;

                if (map[y][x] == PLATFORM) {
                    // Caso 1: choque con una plataforma
                    return Position(final_x, final_y);  // devuelvo la posición actual
                }
                else if (bullet_hit_other_duck(map[y][x], '0')) {
                    // Caso 2: choque con otro pato
                    // avanzo una posición más para que la bala quede "dentro" del pato
                    final_x = next_x;
                    final_y = next_y;
                    return Position(final_x, final_y);
                } else if (map[y][x] == VOID) {
                    // Caso 1: choque con una plataforma
                    hit_void = true;
                    return Position(final_x, final_y);  // devuelvo la posición actual
                }
            }
        }

        if (next_x == new_position.x){
            dx = 0;
        }

        if (next_y == new_position.y){
            dy = 0;
        }

        //si el área está libre, actualizo la posición final
        final_x = next_x;
        final_y = next_y;
        
    }

    return Position(final_x, final_y);
}

// La idea es, muevo a la banana como parabola, y después si cae al piso ya no se mueve
Position GameMap::try_move_banana(Position old_position, Position speed, bool& hit_void) {
    // Si está tocando el piso ya no se mueve más
    if (is_throwable_touching_floor(old_position, 1, 1)) {
        return old_position;
    }
    int final_x = old_position.x;
    int final_y = old_position.y;

    //determino la direccion del movimiento
    int dx = (speed.x > 0) ? 1 : ((speed.x < 0) ? -1 : 0);
    int dy = (speed.y > 0) ? 1 : ((speed.y < 0) ? -1 : 0);

    Position new_position(old_position.x + speed.x, old_position.y + speed.y);

    //itero hasta llegar a la posición final, encontrar un obstaculo o pegarle a un pato
    while (final_x != new_position.x || final_y != new_position.y) {
        //std::cout << "dx es " << dx << " y dy es " << dy << std::endl;
        
        int next_x = final_x + dx;
        int next_y = final_y + dy;

        //verifico si la nueva posición está dentro de los límites del mapa
        if (next_x < 0 || next_x + BULLET_SIZE_X > width || next_y < 0 || next_y + BULLET_SIZE_Y > height) {
            break;
        }

        
        for (int y = next_y; y < next_y + BULLET_SIZE_Y; ++y) {
            for (int x = next_x; x < next_x + BULLET_SIZE_X; ++x) {

                if (map[y][x] == PLATFORM) {
                    // Caso 1: choque con una plataforma
                    return Position(final_x, final_y);  // devuelvo la posición actual
                }
                else if (bullet_hit_other_duck(map[y][x], '0')) {
                    // Caso 2: choque con otro pato
                    // avanzo dos posisiones más para que la bala quede "dentro" del pato
                    
                    final_x = next_x;
                    final_y = next_y;
                    return Position(final_x, final_y);
                } else if (map[y][x] == VOID) {
                    // Caso 1: choque con una plataforma
                    hit_void = true;
                    return Position(final_x, final_y);  // devuelvo la posición actual
                }

            }
        }

        if (next_x == new_position.x){
            dx = 0;
        }

        if (next_y == new_position.y){
            dy = 0;
        }

        //si el área está libre, actualizo la posición final
        final_x = next_x;
        final_y = next_y;
        
    }

    return Position(final_x, final_y);
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
                if (map[y][x] != EMPTY && map[y][x] != duck_id && map[y][x] != BANANA) {
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

    // Verifica si el pato esta sobre el vacio
    if (map[y + DUCK_SIZE_Y][x] == VOID) {
        return true;
    }

    return false;
}

bool GameMap::duckIsOverBullet(Position position) {
    // Verifica si el pato fue golpeado por una bala
    for (int i = position.x; i < position.x + DUCK_SIZE_X; ++i) {
        for (int j = position.y; j < position.y + DUCK_SIZE_Y; ++j) {
            if (map[j][i] == BULLET) {
                return true;
            }
        }
    }

    return false;
}

bool GameMap::duckIsOverBanana(Position position, Position &banana_position) {
    // Verifica si el pato pisa una banana
    for (int i = position.x; i < position.x + DUCK_SIZE_X; ++i) {
        for (int j = position.y; j < position.y + DUCK_SIZE_Y; ++j) {
            if ((j >= 0 && j < height && i >= 0 && i < width) && map[j][i] == BANANA) {
                banana_position = Position {i, j};
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

    //Verifica que no colisione con una BOX
    for (int i = x; i < x + DUCK_SIZE_X; ++i) {
        for (int j = y; j < y + DUCK_SIZE_Y; ++j) {
            if (map[j][i] == BOX) {
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
bool GameMap::can_move_projectile(Position position, int size_x, int size_y) {

    // Verifica que las coordenadas estén dentro de los límites del mapa
    if (position.x < 0 || position.x >= width || position.y < 0 || position.y >= height) {
        return false; 
    }

    // Verifica que el movimiento no colisione con una plataforma
    for (int i = position.x; i < position.x + size_x; ++i) {
        for (int j = position.y; j < position.y + size_y; ++j) {
            if (map[j][i] == PLATFORM) {
                return false;
            }
        }
    }
    
    return true;
}

void GameMap::clean_projectile_old_position(Position position, int size_x, int size_y) {
    for (int i = position.x; i < position.x + size_x; ++i) { 
        for (int j = position.y; j < position.y + size_y; ++j) {
            if (j >= 0 && j < height && i >= 0 && i < width) { // Verificar límites
                map[j][i] = EMPTY; 
            }
        }
    }
}

void GameMap::set_projectile_new_position(Position position,  int size_x, int size_y, char letter) {
    for (int i = position.x; i < position.x + size_x; ++i) {
        for (int j = position.y; j < position.y + size_y; ++j) {
            if (j >= 0 && j < height && i >= 0 && i < width) {
                map[j][i] = letter; // Cambiar, deberías recibir la letra asignada al proyectil
            }
        }
    }
}

void GameMap::move_projectile(Position position, Position speed, int size_x, int size_y) {
    // Gravity check. Can be modularized
    Position gravity_position = Position {position.x, position.y + 1};
    if (can_move_projectile(gravity_position, size_x, size_y)) { // check if use position_x or delta_x
        clean_projectile_old_position(position, size_x, size_y);

        position = gravity_position;

        set_projectile_new_position(position, size_x, size_y, BULLET);
    }

    Position delta = Position {position.x + speed.x, position.y + speed.y};

    if (can_move_projectile(delta, size_x, size_y)) {
        clean_projectile_old_position(position, size_x, size_y);

        position = delta;

        set_projectile_new_position(position, size_x, size_y, BULLET);
    }
    //printMap();
}

// Es copiada de la de bullet pero incluye el size del proyectil
Position GameMap::try_move_projectile_to(Position old_position, Position new_position, int size_x, int size_y, char duck_id, bool& hit_something) {
    int final_x = old_position.x;
    int final_y = old_position.y;

    //determino la direccion del movimiento
    int dx = (new_position.x > old_position.x) ? 1 : (new_position.x < old_position.x ? -1 : 0);
    int dy = (new_position.y > old_position.y) ? 1 : (new_position.y < old_position.y ? -1 : 0);
    

    //itero hasta llegar a la posición final, encontrar un obstaculo o pegarle a un pato
    while (final_x != new_position.x || final_y != new_position.y) {
        //std::cout << "dx es " << dx << " y dy es " << dy << std::endl;
        
        int next_x = final_x + dx;
        int next_y = final_y + dy;

        //verifico si la nueva posición está dentro de los límites del mapa
        if (next_x < 0 || next_x + size_x > width || next_y < 0 || next_y + size_y > height) {
            break;
        }

        
        for (int y = next_y; y < next_y + size_y; ++y) {
            for (int x = next_x; x < next_x + size_x; ++x) {

                if (map[y][x] == PLATFORM) {
                    // Caso 1: choque con una plataforma
                    hit_something = true;
                    return Position(final_x, final_y);  // devuelvo la posición actual
                }
                else if (bullet_hit_other_duck(map[y][x], duck_id)) {
                    // Caso 2: choque con otro pato
                    hit_something = true;
                    // avanzo una posición más para que la bala quede "dentro" del pato
                    final_x = next_x;
                    final_y = next_y;
                    return Position(final_x, final_y);
                }
            }
        }

        if (next_x == new_position.x){
            dx = 0;
        }

        if (next_y == new_position.y){
            dy = 0;
        }

        //si el área está libre, actualizo la posición final
        final_x = next_x;
        final_y = next_y;
        
    }

    return Position(final_x, final_y);

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

void GameMap::clear_map() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            map[y][x] = EMPTY;
        }
    }
}


void GameMap::set_escenario_for_round(MapConfig mapConfig){
    height = mapConfig.map_height;
    width = mapConfig.map_width;
    map = mapConfig.map;

    //hago que la ultima fila sea VACIO
    for (int x = 0; x < width; ++x) {
        map[height - 1][x] = VOID;
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
bool GameMap::placeDuck(Position pos, char duck_id) {

    for (int i = pos.x; i < pos.x + DUCK_SIZE_X; ++i) {
        for (int j = pos.y; j < pos.y + DUCK_SIZE_Y; ++j) {
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

// METODO TEMPORAL: Imprime el mapa en la consola
void GameMap::tellMap() const {
    std:: cout << "Imprimo el mapa: \n";

    bool pato1 = false;
    bool pato2 = false;

    // veo que no haya obstaculos
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (!pato1 && map[j][i] == '1') {
                pato1 = true;
                std:: cout << "pato 1 desde x: " << i << "y: " << j << "\n";
            }
            if (!pato2 && map[j][i] == '2') {
                pato2 = true;
                std:: cout << "pato 2 desde x: " << i << "y: " << j << "\n";
            }

            if (map[j][i] == '*') {
                std:: cout << "Bala encontrada en el x: " << i << "y: " << j << "\n";
            }
        }
    }

    std:: cout << "\n" << "\n";

}

std::vector<std::vector<char>> GameMap::getMap(){
    return map;
}



void GameMap::placeBox(Position pos) {
    for (int y = pos.y; y < pos.y + BOX_SIZE_Y; ++y) {
        for (int x = pos.x; x < pos.x + BOX_SIZE_X; ++x) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                map[y][x] = BOX;
            }
        }
    }
}

bool GameMap::isCollisionWithBox(Position pos, int size_x, int size_y) {
    for (int y = pos.y; y < pos.y + size_y; ++y) {
        for (int x = pos.x; x < pos.x + size_x; ++x) {
            if (x >= 0 && x < width && y >= 0 && y < height && map[y][x] == BOX) {
                return true;
            }
        }
    }
    return false;
}


bool GameMap::boxIsOverBullet(Position position) {

    // Verifica si la caja fue golpeado por una bala
    for (int i = position.x; i < position.x + BOX_SIZE_X; ++i) {
        for (int j = position.y; j < position.y + BOX_SIZE_Y; ++j) {
            if (map[j][i] == BULLET) {
                return true;
            }
        }
    }

    return false;
}

void GameMap::removeBox(Position pos) {
    for (int y = pos.y; y < pos.y + BOX_SIZE_Y; ++y) {
        for (int x = pos.x; x < pos.x + BOX_SIZE_X; ++x) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                map[y][x] = EMPTY;
            }
        }
    }
}
