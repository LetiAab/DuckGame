#include "grenade_fragment.h"

#define GRENADE_FRAGMENT_SIZE_X 1
#define GRENADE_FRAGMENT_SIZE_Y 1

GrenadeFragment::GrenadeFragment(int fragment_id, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance, bool horizontal) 
    : Projectile(fragment_id, 0, position, direction_x, direction_y, map, duck_id, alcance, horizontal) {}
// Nota: deberías diferenciar entre el número de bala y el tipo de proyectil


void GrenadeFragment::comenzar_trayectoria() {
    // Capaz debería no hacer esto para que le peguen 
    // los fragmentos al que le explota en la mano
    update_position();
}

void GrenadeFragment::update_position() {

    if (should_erase) {
        std::cout << "Debería eliminar este fragmento pero lo estoy actualizando, return \n";
        return;
    }

    if(alcance <= 0){
        //si recorrio su maximo tiene que frenar
        impacto = true;
    } else {

        //std::cout << "Comienzo trayectoria desde x: " << position.x << " e y: " << position.y << std::endl;
        //std::cout << "Speed x: " << speed.x << " y: " << speed.y << "\n";

        old_position = position;

        Position delta = Position {position.x + speed.x, position.y + speed.y};
        position = map->try_move_projectile_to(position, delta, GRENADE_FRAGMENT_SIZE_X, GRENADE_FRAGMENT_SIZE_Y, duck_id, impacto);
        
        map->clean_projectile_old_position(old_position, GRENADE_FRAGMENT_SIZE_X, GRENADE_FRAGMENT_SIZE_Y);
        map->set_projectile_new_position(position, GRENADE_FRAGMENT_SIZE_X, GRENADE_FRAGMENT_SIZE_Y, '*');
        
        alcance -= (std::abs(speed.x) + std::abs(speed.y));
    }

}

bool GrenadeFragment::get_fragment_message(Message& msg){
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
    //mandar flag del impacto (?)

    return true;

}

void GrenadeFragment::cleanPostImpacto(){
    map->clean_projectile_old_position(position, GRENADE_FRAGMENT_SIZE_X, GRENADE_FRAGMENT_SIZE_Y);
}

void GrenadeFragment::impactar(){
    impacto = true;
}


bool GrenadeFragment::should_erase_fragment(){
    return should_erase;
}

Position GrenadeFragment::get_position() {
    return position;
}

Position GrenadeFragment::get_speed() {
    return speed;
}
