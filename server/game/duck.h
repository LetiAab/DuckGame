#include <cstdint>
#include "game_map.h"
#include "common/constants.h"
#include "common/position.h"
#ifndef DUCK_H
#define DUCK_H

class Duck {
private:
    char id_player;  // ID del jugador
    int position_x;      // Posición en el eje X
    int position_y;      // Posición en el eje Y
    Position position;
    GameMap& map;   // Referencia al GameMap


public:
    bool is_moving;
    int speed_x;
    int speed_y;
    char looking;
    bool is_jumping;
    bool updated;

    Duck(char id, int x, int y, GameMap& map);

//    void update_position(int delta_x, int delta_y);

    void update_position_speed();
    void update_position();
    void check_gravity();
    bool is_in_air();

    char get_id() const;
    int get_x() const;
    int get_y() const;
};

#endif // DUCK_H
