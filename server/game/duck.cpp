#include "duck.h"

Duck::Duck(char id, int x, int y, GameMap& map) : id_player(id), position_x(x), position_y(y), old_position_x(x), old_position_y(y), 
    map(map),is_moving(false), speed_x(0), speed_y(0), gun(NULL), is_fluttering(false), is_slippy(false), life_points(100), stop_notificated(false) {}



void Duck::update_position_speed() {
    // Tener en cuenta el pato y no solo el punto

    // Gravity check. Can be modularized
    if (map.canMoveDuckTo(position_x, position_y + 1, id_player)) { // check if use position_x or delta_x
        map.cleanDuckOldPosition(position_x, position_y);

        // TODO: acá agregar la verificación del is_fluttering, pero necesitaría aumentar la velocidad de la gravedad
        
        //ROMPE
        /*position_y += 2;
        if (is_fluttering) {
            position_y -= 1;
        }
        */

        position_y += 1;

        map.setDuckNewPosition(position_x, position_y, id_player);
    }



    int delta_x = position_x + speed_x;
    int delta_y = position_y + speed_y;

    if (map.canMoveDuckTo(delta_x, delta_y, id_player)) {
        map.cleanDuckOldPosition(position_x, position_y);

        position_x = delta_x;
        position_y = delta_y;

        map.setDuckNewPosition(delta_x, delta_y, id_player);
    }
    //map.printMap();

}

bool Duck::is_touching_floor() {
    return map.is_duck_touching_floor(position_x, position_y);    
}

char Duck::get_id() const {
    return id_player;
}

int Duck::get_x() const {
    return position_x;
}

int Duck::get_y() const {
    return position_y;
}

int Duck::get_old_x() const {
    return old_position_x;
}

int Duck::get_old_y() const {
    return old_position_y;
}

void Duck::set_old_x(int x)  {
    old_position_x = x;
}

void Duck::set_old_y(int y)  {
    old_position_y = y;
}

void Duck::disparar() {
    int direccion_x = (looking == LOOKING_RIGHT) ? 1 : -1;  
    int direccion_y = 0;  // La bala se mueve horizonalmente

    Bullet nueva_bala(position_x, position_y, direccion_x, direccion_y, &map, id_player);
    nueva_bala.comenzar_trayectoria();
    bullets.push_back(nueva_bala);

}