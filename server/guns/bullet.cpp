#include "bullet.h"

Bullet::Bullet(int bullet_id, int start_x, int start_y, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance) 
    : bullet_id(bullet_id), position(start_x, start_y), direction_x(direction_x), direction_y(direction_y), map(map), impacto(false), duck_id(duck_id), alcance(alcance) {
    // La bala comienza en la posición dada con la dirección especificada
    speed_x = direction_x;
    speed_y = direction_y;
}

void Bullet::comenzar_trayectoria() {
    update_position();
}

void Bullet::update_position() {

    if(alcance == 0){
        //si recorrio su maximo tiene que frenar
        impacto = true;
    }
    std::cout << "Comienzo trayectoria desde x: " << position.x << "\n";

    //map->cleanBulletOldPosition(position_x, position_y);
    int old_position_x = position.x;
    int old_position_y = position.y;


    int delta_x = position.x + speed_x;
    int delta_y = position.y + speed_y;

    if (map->canMoveBulletTo(delta_x, delta_y, duck_id)) { 
        position.x = delta_x;
        position.y = delta_y;
        
        map->setBulletNewPosition(position.x, position.y);
        map->cleanBulletOldPosition(old_position_x, old_position_y); 
        alcance--;
    } else {
        map->cleanBulletOldPosition(position.x, position.y);
        impacto = true;
    }


}

bool Bullet::get_bullet_message(Message& msg){
    //TODO: Hacer el chequeo de si debo mandar mensaje o no, y devolver false sino
    msg.type = BULLET_POS_UPDATE;
    msg.player_id =static_cast<uint16_t>(duck_id - '0');
    msg.bullet_x = position.x;
    msg.bullet_y = position.y;
    msg.bullet_id = bullet_id;

    return true;

}

void Bullet::cleanPostImpacto(){
    map->cleanBulletOldPosition(position.x, position.y);
}

void Bullet::impactar(){
    impacto = true;
}

bool Bullet::hubo_impacto(){
    return impacto;
}

Position Bullet::get_position() {
    return position;
}

