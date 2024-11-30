#ifndef BULLET_H
#define BULLET_H

#include "common/message.h"
#include "common/constants.h"
#include "../game/game_map.h"
#include "projectile.h"

#include <iostream>
#include <cmath> 

class GameMap;
class Bullet : public Projectile {
public:
    Bullet(int bullet_id, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance, bool horizontal);
    void comenzar_trayectoria() override;
    void update_position() override;

    bool should_erase_bullet();
    bool should_erase_projectile() override {
        return should_erase_bullet();
    };

    void cleanPostImpacto() override;
    void impactar() override;

    bool get_bullet_message(Message& msg);

    bool get_projectile_message(Message& msg) override {
        return get_bullet_message(msg);
    };


    Position get_position() override;
    Position get_speed() override;

};

#endif // BULLET_H

