#include "duck.h"
#include <iostream>

Duck::Duck(char id, int x, int y, GameMap& map) :
id_player(id), position_x(x), position_y(y), position(x, y), map(map),
is_moving(false), speed_x(0), speed_y(0), looking(LOOKING_RIGHT), is_jumping(false),
is_fluttering(false), updated(false) {}

bool Duck::is_in_air(){
    return map.canMoveDuckTo(position.x, position.y + 1, id_player);

}

void Duck::check_gravity(){

    if(is_in_air()) {
        speed_y += 2;
        
    }

}

void Duck::update_position_speed() {
    //TODO: TENER EN CUENTA QUE TENGO QUE VER TODO LO QUE OCUPA EL PATO Y NO SOLO EL PUNTO QUE GUARDA

    // Gravity check. Can be modularized
    bool can_fall = map.canMoveDuckTo(position_x, position_y + 1, id_player);
    if(can_fall) {
        map.cleanDuckOldPosition(position_x, position_y);
        speed_y +=2;

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

}


void Duck::update_position() {

    check_gravity();
    
    int delta_x = position.x + speed_x;
    int delta_y = position.y + speed_y;

    std::cout << "Posición antes de mover: (" << position.x << ", " << position.y << ")" << std::endl;

    Position new_pos(delta_x, delta_y);
    //mueve al pato a la nueva posicion si esta libre o a la que este libre inmediatamente antes
    position = map.move_duck_to(position, new_pos, id_player);

    std::cout << "Speed X: " << speed_x << ", Speed Y: " << speed_y << std::endl;
    std::cout << "Posición despues de mover: (" << position.x << ", " << position.y << ")" << std::endl;
    map.printMap();

    if ((is_jumping || is_fluttering) && !is_in_air()){
        //si esta saltando o aleteando pero no esta en el aire, significa que aterrizo
        is_jumping = false;
        is_fluttering = false;
        speed_y = 0;
    }

    if(!is_in_air()){
        speed_y = 0;
    }

}


char Duck::get_id() const {
    return id_player;
}

int Duck::get_x() const {
    return position.x;
}

int Duck::get_y() const {
    return position.y;
}
