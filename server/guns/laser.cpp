#include "laser.h"
#include <cmath> 

Laser::Laser(int laser_id, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance) 
    : laser_id(laser_id),
    position(position),
    old_position(position),
    speed(direction_x, direction_y),
    direction_x(direction_x),
    direction_y(direction_y),
    map(map),
    impacto(false),
    duck_id(duck_id),
    alcance(alcance),
    should_erase(false) {}



void Laser::comenzar_trayectoria() {
    update_position();
}

void Laser::update_position() {

    if(alcance <= 0){
        //si recorrio su maximo tiene que frenar
        impacto = true;
    } else {

        std::cout << "Comienzo trayectoria desde x: " << position.x << "\n";

        int delta_x = position.x + speed.x;
        int delta_y = position.y + speed.y;
        old_position = position;

        position = map->try_move_bullet_to(position, Position(delta_x, delta_y), duck_id, impacto);
        
        map->cleanBulletOldPosition(old_position);
        map->setBulletNewPosition(position);
        
        //por ahora, le resto la velocidad en x ya que solo dispara en horizontal
        alcance -= std::abs(speed.x);
    }

}

bool Laser::get_laser_message(Message& msg){

    if(impacto){
        //si empacte con algo debo eliminar la bala luego de mandar el mensaje
        cleanPostImpacto();
        should_erase = true;
    }

    if(position.is_same_position(old_position) ){
        return false;
    }

    msg.type = BULLET_POS_UPDATE;
    msg.player_id =static_cast<uint16_t>(duck_id - '0');
    msg.bullet_x = position.x;
    msg.bullet_y = position.y;
    msg.bullet_id = laser_id;
    //mandar flag del impacto (?)

    return true;

}

void Laser::cleanPostImpacto(){
    map->cleanBulletOldPosition(position);
}

void Laser::impactar(){
    impacto = true;
}


bool Laser::should_erase_laser(){
    return should_erase;
}

Position Laser::get_position() {
    return position;
}

Position Laser::get_speed() {
    return speed;
}
