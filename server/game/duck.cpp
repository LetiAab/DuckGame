#include "duck.h"

Duck::Duck(char id, int x, int y, GameMap& map) : id_player(id), position_x(x), position_y(y), map(map), is_moving(false), speed_x(0), speed_y(0) {}

/* void Duck::update_position(int delta_x, int delta_y) {

    //TODO: TENER EN CUENTA QUE TENGO QUE VER TODO LO QUE OCUPA EL PATO Y NO SOLO EL PUNTO QUE GUARDA
    int old_position_x = position_x;
    int old_position_y = position_y;


    if (map.canMoveDuckTo(delta_x, delta_y)){
        position_x = delta_x; 
        position_y = delta_y; 
    
        map.cleanDuckOldPosition(old_position_x, old_position_y);
        map.setDuckNewPosition(delta_x, delta_y);

        map.printMap();

    }
} */

void Duck::update_position_speed() {
    //TODO: TENER EN CUENTA QUE TENGO QUE VER TODO LO QUE OCUPA EL PATO Y NO SOLO EL PUNTO QUE GUARDA

    // Gravity check. Can be modularized
    if (map.canMoveDuckTo(position_x, position_y + 1, id_player)) { // check if use position_x or delta_x
        map.cleanDuckOldPosition(position_x, position_y);

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
    map.printMap();

    //speed_x = 0;
    //speed_y = 0;
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
