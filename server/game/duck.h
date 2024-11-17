#ifndef DUCK_H
#define DUCK_H


#include <cstdint>
#include "game_map.h"
#include "../guns/projectile.h"
#include "common/constants.h"
#include "common/position.h"
#include "common/message.h"


#include <string>

#include "../guns/bullet.h"
#include "../guns/weapon.h"
#include "../guns/pew_pew_laser.h"
#include "../guns/laser_rifle.h"
#include "../guns/ak_47.h"
#include "../guns/duel_pistol.h"
#include "../armor/armor.h"
#include "../armor/helmet.h"


class Arma;
class GameMap;
class Bullet;  // Declaración adelantada de Bullet
class Weapon;  // Declaración adelantada de Weapon
class Armor;  // Declaración adelantada de Weapon
class Helmet;  // Declaración adelantada de Weapon
class Ak47;
class DuelPistol;


class Duck {
private:
    char id_player;  // ID del jugador
    Position position;
    Position old_position;
    GameMap* map;   // Referencia al GameMap

    void form_position_message(Message& msg);
    void check_gravity();


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
    bool is_dead;
    DuelPistol* weapon;
    Armor* armor;
    Helmet* helmet;
    
    std::unique_ptr<Item> onHand;  // Usamos un smart pointer para manejar el recurso

    Duck(char id, int x, int y, GameMap* map);

    void update_position();

    int update_life();

    void update_weapon();

    bool is_in_air();

    bool get_duck_position_message(Message& msg);

    bool get_duck_dead_message(Message& msg);

    bool get_duck_broke_helmet_message(Message& msg);

    bool get_duck_broke_armor_message(Message& msg);

    char get_id() const;

    void setWeapon(DuelPistol* new_weapon);
    void setArmor(Armor* new_armor);
    void setHelmet(Helmet* new_helmet);

    bool dropWeapon();

    bool pickUpItem(Item* item);
    void useOnHand();
    Item* getItemOnHand() const;

    void disparar();

    Position getPosition();

};

#endif // DUCK_H
