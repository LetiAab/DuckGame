#include "bouncing_laser.h"

BouncingLaser::BouncingLaser(int laser_id, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance) 
    : Projectile(laser_id, 2, position, direction_x, direction_y, map, duck_id, alcance) {}
// Nota: Para dibujar el laser necesitaría un manejo de grados, lo cual no es tan complicado, pero no lo uso para caluclar la posición


void BouncingLaser::comenzar_trayectoria() {
    update_position();
}

void BouncingLaser::update_position() {

    if(alcance <= 0){
        //si recorrio su maximo tiene que frenar
        impacto = true;
    } else {

        std::cout << "Comienzo trayectoria desde x: " << position.x << "\n";

        int delta_x = position.x + speed.x;
        int delta_y = position.y + speed.y;
        old_position = position;

        bool hit_platform = false;
        bool hit_x = false;

        position = map->try_move_bouncing_laser_to(position, Position(delta_x, delta_y), duck_id, impacto, hit_platform, hit_x);
        if (hit_platform) {
                if (hit_x) {
                        speed.x = 0 - speed.x;
                } else {
                        speed.y = 0 - speed.y;
                }
        }
        hit_platform = false;
        
        map->cleanBulletOldPosition(old_position);
        map->setBulletNewPosition(position);
        
        //por ahora, le resto la velocidad en x ya que solo dispara en horizontal
        alcance -= std::abs(speed.x);
    }

}

bool BouncingLaser::get_laser_message(Message& msg){

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
    //mandar flag del impacto (?)

    return true;
}

void BouncingLaser::cleanPostImpacto(){
    map->cleanBulletOldPosition(position);
}

void BouncingLaser::impactar(){
    impacto = true;
}


bool BouncingLaser::should_erase_laser(){
    return should_erase;
}

Position BouncingLaser::get_position() {
    return position;
}

Position BouncingLaser::get_speed() {
    return speed;
}
