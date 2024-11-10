#include <cstdint>
#include "game_map.h"
#include "../guns/projectile.h"
#include "common/constants.h"
#ifndef DUCK_H
#define DUCK_H

#include <string>

class Arma;
class GameMap;

class Duck {
private:
    char id_player;  // ID del jugador
    int position_x;      // Posición en el eje X
    int position_y;      // Posición en el eje Y
    GameMap& map;   // Referencia al GameMap


public:
    bool is_moving;
    int speed_x;
    int speed_y;
    Arma* gun;
    bool is_fluttering;
    bool is_slippy;
    int life_points;
    char looking;

    Duck(char id, int x, int y, GameMap& map);

//    void update_position(int delta_x, int delta_y);

    void update_position_speed();

    bool is_touching_floor();

    char get_id() const;
    int get_x() const;
    int get_y() const;
};

#endif // DUCK_H
