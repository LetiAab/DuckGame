#include "projectile.h"

#include "../game/game.h"

Projectile::Projectile(int projectile_id, int projectile_type, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance, bool horizontal) 
    : projectile_id(projectile_id),
    projectile_type(projectile_type),
    position(position),
    old_position(position),
    speed(direction_x, direction_y),
    direction_x(direction_x),
    direction_y(direction_y),
    map(map),
    impacto(false),
    duck_id(duck_id),
    alcance(alcance),
    should_erase(false),
    horizontal(horizontal) {}
