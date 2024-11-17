#ifndef BULLET_H
#define BULLET_H

#include "common/message.h"
#include "common/constants.h"
#include "../game/game_map.h"

#include <iostream>
#include <cmath> 

class GameMap;
class Bullet {
private:
    int bullet_id; //para identificar las balas en los mensajes

    Position position;
    Position old_position;
    Position speed;
    int direction_x; 
    int direction_y;

    GameMap* map;   
    bool impacto;
    char duck_id; //tengo que saber de quien es la bala para que no se autopegue
    int alcance;
    bool should_erase;


public:
    Bullet(int bullet_id, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance);
    void comenzar_trayectoria();
    void update_position();

    bool should_erase_bullet();
    void cleanPostImpacto();
    void impactar();

    bool get_bullet_message(Message& msg);

    Position get_position();

    Position get_speed();


};

#endif // BULLET_H

