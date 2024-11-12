#include "duck.h"
#include <iostream>

Duck::Duck(char id, int x, int y, GameMap* map) :
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
    stop_notificated(false),
    is_dead(false) {}

bool Duck::is_in_air(){
    std::cout << "ME FIJO SI ESTYO EN EL AIRE" << "\n";
    return map->canMoveDuckTo(position.x, position.y + 1, id_player);
}


void Duck::check_gravity(){

    if(is_in_air()) {
        if (is_fluttering){
            speed_y += 1;
        }else {
            speed_y += 2;
        }
        
    }

}

void Duck::update_position() {

    if(map->duckIsOverVoid(position.x, position.y)){
        is_dead = true;
    }

    if (is_dead) {
        map->cleanDuckOldPosition(position.x, position.y);
        return;
    }
    check_gravity();
    
    int delta_x = position.x + speed_x;
    int delta_y = position.y + speed_y;

    std::cout << "Posición antes de mover: (" << position.x << ", " << position.y << ")" << std::endl;

    Position new_pos(delta_x, delta_y);
    old_position = position;
    //mueve al pato a la nueva posicion si esta libre o a la que este libre inmediatamente antes
    position = map->move_duck_to(position, new_pos, id_player);

    std::cout << "Speed X: " << speed_x << ", Speed Y: " << speed_y << std::endl;
    std::cout << "Posición despues de mover: (" << position.x << ", " << position.y << ")" << std::endl;
    //map.printMap();

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

void Duck::form_position_message(Message& msg){
    msg.type = DUCK_POS_UPDATE;
    msg.player_id = static_cast<uint16_t>(id_player - '0'); // Convertimos el id a int
    msg.duck_x = position.x;
    msg.duck_y = position.y;
    msg.looking = looking;
    msg.is_moving = is_moving;
    msg.is_jumping = is_jumping;
    msg.is_fluttering = is_fluttering;
}

bool Duck::get_duck_position_message(Message& msg){
    if (old_position.x == position.x && old_position.y == position.y){
        if(stop_notificated){
            return false;
        } else {

            form_position_message(msg);
            stop_notificated = true;

            return true;
        }
    }

    form_position_message(msg);
    stop_notificated = false;

    return true;
}


char Duck::get_id() const {
    return id_player;
}



void Duck::setWeapon(Weapon* new_weapon) {
    weapon = new_weapon;  // Asigna el arma al pato
}


void Duck::disparar() {

   if (weapon != nullptr) {
        weapon->disparar(position.x, position.y, looking, map, id_player);
    }
}

void Duck::get_hit_by_bullet(Bullet bullet) {
    // Nota: recibo bullet para en un futuro preguntar por cuanto daño hace o algo del estilo, ahora no se usa

    std::cout << "Me dio una bala, -20, x=" << bullet.get_position().x << std::endl;


    life_points -= 20;
    if (life_points <= 0) {
        is_dead = true;
        std::cout << "Ahora estoy muerto, me dio una bala :(" << std::endl;
        map->cleanDuckOldPosition(position.x, position.y);
        
    }
}

