#ifndef BULLET_H
#define BULLET_H

#include "../game/game_map.h"

class GameMap;
class Bullet {
private:
    int position_x;
    int position_y;
    int speed_x;
    int speed_y;
    int direction_x; 
    int direction_y;

    GameMap* map;   
    bool impacto;
    char duck_id; //tengo que saber de quien es la bala para que no se autopegue cuando sale

    int alcance;


public:
    Bullet(int start_x, int start_y, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance);
    void comenzar_trayectoria();  
    void update_position();       
    int get_x() const;            
    int get_y() const;           

    bool hubo_impacto();
    void cleanPostImpacto();
    void impactar();

    char getDuckId() const;


};

#endif // BULLET_H
