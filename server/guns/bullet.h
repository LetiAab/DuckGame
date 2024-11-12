#ifndef BULLET_H
#define BULLET_H

#include "common/message.h"
#include "common/constants.h"
#include "../game/game_map.h"

class GameMap;
class Bullet {
private:
    int bullet_id; //para identificar las balas en los mensajes

    Position position;
    Position speed;
    int direction_x; 
    int direction_y;

    GameMap* map;   
    bool impacto;
    char duck_id; //tengo que saber de quien es la bala para que no se autopegue cuando sale

    int alcance;


public:
    Bullet(int bullet_id, int start_x, int start_y, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance);
    void comenzar_trayectoria();
    void update_position();

    bool hubo_impacto();
    void cleanPostImpacto();
    void impactar();

    bool get_bullet_message(Message& msg);

    Position get_position();

    Position get_speed();


};

#endif // BULLET_H

