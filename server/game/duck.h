#include <cstdint>
#include "game_map.h"
#include "../guns/projectile.h"
#include "common/constants.h"
#include "common/position.h"
#include "common/message.h"

#ifndef DUCK_H
#define DUCK_H

#include <string>

#include "../guns/bullet.h"
#include "../guns/weapon.h"

class Arma;
class GameMap;
class Bullet;  // Declaración adelantada de Bullet
class Weapon;  // Declaración adelantada de Weapon


class Duck {
private:
    char id_player;  // ID del jugador
    Position position;
    Position old_position;
    GameMap& map;   // Referencia al GameMap


    void form_position_message(Message& msg);


public:
    bool is_moving;
    int speed_x;
    int speed_y;
    //Arma* gun;
    char looking;
    bool is_jumping;
    bool is_fluttering;
    bool is_slippy;
    int life_points;
    bool stop_notificated;
    Weapon* weapon;

    
    Duck(char id, int x, int y, GameMap& map);

    void update_position();

    void check_gravity();

    bool is_in_air();

    bool get_duck_position_message(Message& msg);

    char get_id() const;

    void setWeapon(Weapon* new_weapon);

    void disparar();
};

#endif // DUCK_H
