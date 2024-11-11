#include "duck.h"
#include <iostream>

Duck::Duck(char id, int x, int y, GameMap& map) :
    id_player(id),
    position(x, y),
    old_position(x, y),
    map(map),
    is_moving(false),
    speed_x(0),
    speed_y(0),
    looking(LOOKING_RIGHT),
    is_jumping(false),
    is_fluttering(false),
    is_slippy(false),
    life_points(100),
    stop_notificated(false) {}

bool Duck::is_in_air(){
    return map.canMoveDuckTo(position.x, position.y + 1, id_player);
}


void Duck::check_gravity(){

    if(is_in_air()) {
        speed_y += 2;
        
    }

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

int Duck::get_old_x() const {
    return old_position.x;
}

int Duck::get_old_y() const {
    return old_position.y;
}

void Duck::set_old_x(int x)  {
    old_position.x = x;
}

void Duck::set_old_y(int y)  {
    old_position.y = y;
}




/*
void Duck::update_position_speed() {
    // Tener en cuenta el pato y no solo el punto

    // Gravity check. Can be modularized
    bool can_fall = map.canMoveDuckTo(position.x, position.y + 1, id_player);
    if(can_fall) {
        map.cleanDuckOldPosition(position.x, position.y);

        position.y += 1;

        map.setDuckNewPosition(position.x, position.y, id_player);
    }


    int delta_x = position.x + speed_x;
    int delta_y = position.y + speed_y;

    if (map.canMoveDuckTo(delta_x, delta_y, id_player)) {
        map.cleanDuckOldPosition(position.x, position.y);

        position.x = delta_x;
        position.y = delta_y;

        map.setDuckNewPosition(delta_x, delta_y, id_player);
    }
    //map.printMap();

}
*/

void Duck::setWeapon(Weapon* new_weapon) {
    weapon = new_weapon;  // Asigna el arma al pato
}


void Duck::disparar() {

   if (weapon != nullptr) {
        weapon->disparar(position.x, position.y, looking, map, id_player);
    }
}

