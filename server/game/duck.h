#include <cstdint>
#include "game_map.h"
#ifndef DUCK_H
#define DUCK_H

class Duck {
private:
    uint16_t id_player;  // ID del jugador
    int position_x;      // Posición en el eje X
    int position_y;      // Posición en el eje Y
    GameMap& map;   // Referencia al GameMap


public:
    Duck(uint16_t id, int x, int y, GameMap& map);

    void update_position(int delta_x, int delta_y);

    uint16_t get_id() const;
    int get_x() const;
    int get_y() const;
};

#endif // DUCK_H
