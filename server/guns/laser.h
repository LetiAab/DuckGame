#ifndef LASER_H
#define LASER_H

#include "common/message.h"
#include "common/constants.h"
#include "../game/game_map.h"

class GameMap;
class Laser {
private:
    int laser_id; //para identificar las balas en los mensajes

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
    Laser(int laser_id, int start_x, int start_y, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance);
    void comenzar_trayectoria();
    void update_position();

    bool should_erase_laser();
    void cleanPostImpacto();
    void impactar();

    bool get_laser_message(Message& msg);

    Position get_position();

    Position get_speed();


};

#endif // LASER_H

