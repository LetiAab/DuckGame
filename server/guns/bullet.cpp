#include "bullet.h"
#include <cmath> 

Bullet::Bullet(int bullet_id, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance) 
    : bullet_id(bullet_id),
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



void Bullet::comenzar_trayectoria() {
    update_position();
}

void Bullet::update_position() {

    if(alcance <= 0){
        //si recorrio su maximo tiene que frenar
        impacto = true;
    } else {

        std::cout << "Comienzo trayectoria desde x: " << position.x << "\n";
        std::cout << "Speed x: " << speed.x << " y: " << speed.y << "\n";


        int delta_x = position.x + speed.x;
        int delta_y = position.y + speed.y;
        old_position = position;

        position = map->try_move_bullet_to(position, Position(delta_x, delta_y), duck_id, impacto);

        std::cout << "La posicion despues de intentar mov trayectoria desde x: " << position.x << " y: " << position.y << "\n";

        
        map->cleanBulletOldPosition(old_position);
        map->setBulletNewPosition(position);
        
        //por ahora, le resto la velocidad en x ya que solo dispara en horizontal
        alcance -= (std::abs(speed.x) + std::abs(speed.y));
    }

}

bool Bullet::get_bullet_message(Message& msg){

    if(impacto){
        //si impacte con algo debo eliminar la bala luego de mandar el mensaje
        std::cout << "Llamo a cleanPostImpacto, la bullet se 'borra' en x" << position.x << " y: " << position.y << std:: endl;
        cleanPostImpacto();
        std::cout << "Las dos posiciones de la bala 'borradas' son " << map->at(position) << " y " << map->at(Position {position.x + 1, position.y});
        should_erase = true;
    }

    if(position.is_same_position(old_position) ){
        return false;
    }

    msg.type = BULLET_POS_UPDATE;
    msg.player_id =static_cast<uint16_t>(duck_id - '0');
    msg.bullet_x = position.x;
    msg.bullet_y = position.y;
    msg.bullet_id = bullet_id;
    //mandar flag del impacto (?)

    return true;

}

void Bullet::cleanPostImpacto(){
    std::cout << "En el cleanPostImpacto de bullet, la posición es x" << position.x << " y: " << position.y << std:: endl;
    map->cleanBulletOldPosition(position);
}

void Bullet::impactar(){
    impacto = true;
}


bool Bullet::should_erase_bullet(){
    return should_erase;
}

Position Bullet::get_position() {
    return position;
}

Position Bullet::get_speed() {
    return speed;
}
