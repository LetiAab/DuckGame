#ifndef DUCK_H
#define DUCK_H


#include <cstdint>
#include <string>
#include <condition_variable>

#include "game_map.h"

#include "common/constants.h"
#include "common/position.h"
#include "common/message.h"

#include "../armor/armor.h"
#include "../armor/helmet.h"

#include "../guns/weapon.h"
#include "../guns/ak_47.h"
#include "../guns/magnum.h"
#include "../guns/cowboy_pistol.h"
#include "../guns/duel_pistol.h"
#include "../guns/pew_pew_laser.h"
#include "../guns/laser_rifle.h"
#include "../guns/shotgun.h"
#include "../guns/sniper.h"



class GameMap;
class Item;
class Weapon;
class Bullet;



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
    std::shared_ptr<Weapon> weapon;
    std::shared_ptr<Armor> armor;
    std::shared_ptr<Helmet> helmet;
    
    std::shared_ptr<Item> onHand;  // Usamos un smart pointer para manejar el recurso

    Duck(char id, int x, int y, GameMap* map);

    void update_position();

    int update_life();

    void update_weapon();

    bool is_in_air();

    bool get_duck_position_message(Message& msg);
    bool get_duck_initialize_message(Message& msg);

    bool get_duck_dead_message(Message& msg);

    bool get_duck_broke_helmet_message(Message& msg);

    bool get_duck_broke_armor_message(Message& msg);

    char get_id() const;

    void setWeapon(std::shared_ptr<Weapon> new_weapon);
    void setArmor(std::shared_ptr<Armor> new_armor);
    void setHelmet(std::shared_ptr<Helmet> new_helmet);

    bool dropWeapon();

    bool pickUpItem(std::shared_ptr<Item> item);
    void useOnHand();
    std::shared_ptr<Item> getItemOnHand() const;

    bool disparar();

    void reset_for_round(Position pos);

    Position getPosition();

};

#endif // DUCK_H
