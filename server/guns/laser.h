#ifndef LASER_H
#define LASER_H

#include "common/message.h"
#include "common/constants.h"
#include "../game/game_map.h"
#include "projectile.h"

#include <cmath> 
#include <iostream>

class GameMap;
class Laser : public Projectile{
public:
    Laser(int laser_id, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance, bool horizontal);
    void comenzar_trayectoria() override;
    void update_position() override;

    bool should_erase_laser();
    bool should_erase_projectile() override {
        return should_erase_laser();
    };

    void cleanPostImpacto() override;
    void impactar() override;

    bool get_laser_message(Message& msg);
    bool get_projectile_message(Message& msg) override {
        return get_laser_message(msg);
    };

    Position get_position() override;

    Position get_speed() override;
};

#endif // LASER_H

