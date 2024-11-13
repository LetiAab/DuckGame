#include "bullet.h"

Bullet::Bullet(int bullet_id, int start_x, int start_y, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance) 
    : bullet_id(bullet_id), position(start_x, start_y), speed(direction_x, direction_y), direction_x(direction_x), direction_y(direction_y), 
    map(map), impacto(false), duck_id(duck_id), alcance(alcance) {}

void Bullet::comenzar_trayectoria() {
    if(alcance <= 0){
        //si recorrio su maximo tiene que frenar
        impacto = true;
    }
    std::cout << "Comienzo trayectoria desde x: " << position.x << "\n";


    int delta_x = position.x + speed.x;
    int delta_y = position.y + speed.y;
    Position old_position = position;

    position = map->try_move_bullet_to(position, Position(delta_x, delta_y), duck_id, impacto);

    //impacte contra una pared, por eso estoy en un espacio vacio, asi que borro la bala
    if(impacto && map->at(position) == ' '){
        map->cleanBulletOldPosition(old_position);

    } else {
    //no impacte con nada o impacte con un pato
        map->cleanBulletOldPosition(old_position);
        map->setBulletNewPosition(position);
    }
}

void Bullet::update_position() {

    if(alcance <= 0){
        //si recorrio su maximo tiene que frenar
        impacto = true;
    }
    std::cout << "Comienzo trayectoria desde x: " << position.x << "\n";


    int delta_x = position.x + speed.x / 2;
    int delta_y = position.y + speed.y;
    Position old_position = position;

    position = map->try_move_bullet_to(position, Position(delta_x, delta_y), duck_id, impacto);

    //impacte contra una pared, por eso estoy en un espacio vacio, asi que borro la bala
    if(impacto && map->at(position) == ' '){
        map->cleanBulletOldPosition(old_position);

    } else {
    //no impacte con nada o impacte con un pato
        map->cleanBulletOldPosition(old_position);
        map->setBulletNewPosition(position);
    }

}
/*
void Bullet::update_position() {

    if(alcance == 0){
        //si recorrio su maximo tiene que frenar
        impacto = true;
    }
    std::cout << "Comienzo trayectoria desde x: " << position.x << "\n";

    //map->cleanBulletOldPosition(position_x, position_y);
    int old_position_x = position.x;
    int old_position_y = position.y;


    int delta_x = position.x + speed.x;
    int delta_y = position.y + speed.y;

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


}*/

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

Position Bullet::get_speed() {
    return speed;
}
