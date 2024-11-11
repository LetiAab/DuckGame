#include <cstdint>
#include "game_map.h"
#include "../guns/projectile.h"
#include "common/constants.h"
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
    int position_x;      // Posición en el eje X
    int position_y;      // Posición en el eje Y

    int old_position_x;
    int old_position_y;
    GameMap& map;   // Referencia al GameMap




public:
    bool is_moving;
    int speed_x;
    int speed_y;
    //Arma* gun;
    bool is_fluttering;
    bool is_slippy;
    int life_points;
    char looking;
    bool stop_notificated;
    Weapon* weapon;

    
    
    std::vector<Bullet> bullets;  // Lista de balas disparadas

    Duck(char id, int x, int y, GameMap& map);

//    void update_position(int delta_x, int delta_y);

    void update_position_speed();

    bool is_touching_floor();

    char get_id() const;
    int get_x() const;
    int get_y() const;

    int get_old_x() const;
    int get_old_y() const;

    void set_old_x(int x) ;
    void set_old_y(int y) ;

    void setWeapon(Weapon* new_weapon);

    void disparar();
};

#endif // DUCK_H
