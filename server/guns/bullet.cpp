#include "bullet.h"

Bullet::Bullet(int bullet_id, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance, bool horizontal) 
    : Projectile(bullet_id, 0, position, direction_x, direction_y, map, duck_id, alcance, horizontal) {}
// Nota: deberías diferenciar entre el número de bala y el tipo de proyectil


void Bullet::comenzar_trayectoria() {
    update_position();
}

void Bullet::update_position() {

    if (should_erase) {
        return;
    }

    if(alcance <= 0){
        impacto = true;
    } else {

        int delta_x = position.x + speed.x;
        int delta_y = position.y + speed.y;
        old_position = position;

        position = map->try_move_bullet_to(position, Position(delta_x, delta_y), duck_id, impacto);
        
        map->cleanBulletOldPosition(old_position);
        map->setBulletNewPosition(position);
        
        alcance -= (std::abs(speed.x) + std::abs(speed.y));
    }

}

bool Bullet::get_bullet_message(Message& msg){

    if(impacto){
        //si impacte con algo debo eliminar la bala luego de mandar el mensaje
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
    msg.bullet_id = projectile_id;
    msg.bullet_type = projectile_type;
    msg.bullet_horizontal = horizontal;

    return true;

}

void Bullet::cleanPostImpacto(){
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
