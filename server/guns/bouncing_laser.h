#ifndef BOUNCING_LASER_H
#define BOUNCING_LASER_H

#include "common/message.h"
#include "common/constants.h"
#include "common/position.h"
#include "../game/game_map.h"
#include "projectile.h"

#include <cmath> 
#include <iostream>

class BouncingLaser : public Projectile {
private:

public:
    BouncingLaser(int laser_id, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance);
    void comenzar_trayectoria() override;
    void update_position() override;

    bool should_erase_laser();
    bool should_erase_projectile() override {
        return should_erase_projectile();
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

#endif // BOUNCING_LASER_H
