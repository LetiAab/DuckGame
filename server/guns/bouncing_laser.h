#ifndef BOUNCING_LASER_H
#define BOUNCING_LASER_H

#include "common/message.h"
#include "common/constants.h"
#include "../game/game_map.h"

class GameMap;
class BouncingLaser {
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
    BouncingLaser(int laser_id, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance);
    void comenzar_trayectoria();
    void update_position();

    bool should_erase_laser();
    void cleanPostImpacto();
    void impactar();

    bool get_laser_message(Message& msg);

    Position get_position();

    Position get_speed();


};

#endif // BOUNCING_LASER_H








/* #ifndef BOUNCING_LASER_H
#define BOUNCING_LASER_H

#include "laser.h"
#include "../game/game_map.h"

class BouncingLaser : public Laser {
private:
    int max_rebotes;  // Número máximo de rebotes permitidos
    int rebotes_realizados;  // Contador de rebotes realizados

public:
    BouncingLaser(int laser_id, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance, int max_rebotes);

    void update_position() override;  // Sobrescribir el comportamiento de `update_position`
};

#endif // BOUNCING_LASER_H
 */