#include "bullet.h"

Bullet::Bullet(int bullet_id, int start_x, int start_y, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance) 
    : bullet_id(bullet_id), position_x(start_x), position_y(start_y), direction_x(direction_x), direction_y(direction_y), map(map), impacto(false), duck_id(duck_id), alcance(alcance) {
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
    std::cout << "Comienzo trayectoria desde x: " << position_x << "\n";

    //map->cleanBulletOldPosition(position_x, position_y);
    int old_position_x = position_x;
    int old_position_y = position_y;


    int delta_x = position_x + speed_x;
    int delta_y = position_y + speed_y;

    if (map->canMoveBulletTo(delta_x, delta_y, duck_id)) { 
        position_x = delta_x;
        position_y = delta_y;
        
        map->setBulletNewPosition(position_x, position_y);
        map->cleanBulletOldPosition(old_position_x, old_position_y); 
        alcance--;
    } else {
        map->cleanBulletOldPosition(position_x, position_y);
        impacto = true;
    }


}

void Bullet::cleanPostImpacto(){
    map->cleanBulletOldPosition(position_x, position_y);
}

int Bullet::get_x() const {
    return position_x;
}

int Bullet::get_y() const {
    return position_y;
}

void Bullet::impactar(){
    impacto = true;
}

bool Bullet::hubo_impacto(){
    return impacto;
}

char Bullet::getDuckId() const{
    return duck_id;
}

int Bullet::getBulletId() const{
    return bullet_id;
}
