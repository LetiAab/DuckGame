#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../../common/message.h"
#include "../../common/constants.h"
#include "../game/game_map.h"

class Projectile {
protected:
    int projectile_id;
    int projectile_type;
    Position position;
    Position old_position;
    Position speed;
    int direction_x;
    int direction_y;
    GameMap* map;
    bool impacto;
    char duck_id;
    int alcance;
    bool should_erase;
    bool horizontal;

public:
    Projectile(int projectile_id, int projectile_type, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance, bool horizontal);
    virtual ~Projectile() = default;

    virtual void comenzar_trayectoria() = 0;
    virtual void update_position() = 0;

    virtual bool should_erase_projectile() = 0;
    virtual void cleanPostImpacto() = 0;
    virtual void impactar() = 0;

    virtual bool get_projectile_message(Message& msg) = 0;

    virtual Position get_position() = 0;
    virtual Position get_speed() = 0;
};

#endif // PROJECTILE_H
