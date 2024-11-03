#include "duck.h"

Duck::Duck(uint16_t id, int x, int y, GameMap& map) : id_player(id), position_x(x), position_y(y), map(map), speed_x(0), speed_y(0) {}

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

    if (map.canMoveDuckTo(this->position_x + this->speed_x, this->position_y + this->speed_y)) {
        map.cleanDuckOldPosition(this->position_x, this->position_y);

        this->position_x += this->speed_x; 
        this->position_y += this->speed_y; 

        map.setDuckNewPosition(this->position_x, this->position_y);

        this->speed_x = 0;
        this->speed_y = 0;

        map.printMap();
    }
}


uint16_t Duck::get_id() const {
    return id_player;
}

int Duck::get_x() const {
    return position_x;
}

int Duck::get_y() const {
    return position_y;
}
