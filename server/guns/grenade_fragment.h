#ifndef GRENADE_FRAGMENT_H
#define GRENADE_FRAGMENT_H

#include "common/message.h"
#include "common/constants.h"
#include "../game/game_map.h"
#include "projectile.h"

#include <iostream>
#include <cmath> 

class GameMap;
class GrenadeFragment : public Projectile {
public:
    GrenadeFragment(int fragment_id, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance);
    void comenzar_trayectoria() override;
    void update_position() override;

    bool should_erase_fragment();
    bool should_erase_projectile() override {
        return should_erase_fragment();
    };

    void cleanPostImpacto() override;
    void impactar() override;

    bool get_fragment_message(Message& msg);

    bool get_projectile_message(Message& msg) override {
        return get_fragment_message(msg);
    };


    Position get_position() override;
    Position get_speed() override;

};

#endif // GRENADE_FRAGMENT_H

