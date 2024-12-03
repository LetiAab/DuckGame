#include "shotgun.h"
#include <iostream>

// 2 disparos, Alcance: 7 a 9 tiles (7 x TILE_SIZE = 28)
Shotgun::Shotgun(int x, int y)
    : Weapon(SHOTGUN_ID, "Shotgun", SHOTGUN_RANGE, 0, SHOTGUN_AMMUNITION, x, y), recargando(false) {}

bool Shotgun::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) {
    if (municiones > 0) {
        if (recargando) {
                recargando = false;
                return false;
        }
        //la bala debe aparecer fuera del pato, o sino se mata a si mismo
        int bullet_position_x = (looking == LOOKING_RIGHT) ? position_x + DUCK_SIZE_X : position_x - BULLET_SIZE_X;
        int bullet_position_y = (is_looking_up) ? position_y - BULLET_SIZE_Y : position_y-4;

        Position bullet_pos(bullet_position_x, bullet_position_y);
        //si donde debe salir la bala hay una pared, no puedo disparar
        if(map->at(bullet_pos)== 'P') {
            std::cout << "No puedo disparar, hay una pared inmediatamente al lado" << std::endl;
            return false;
        }

        int direccion_x = (looking == LOOKING_RIGHT) ? BULLET_SPEED_X : -BULLET_SPEED_Y;
        int direccion_y = 0;  // La bala se mueve horizonalmente

        if(is_looking_up){
            direccion_x = 0;
            direccion_y = -BULLET_SPEED_Y;
        }

        for (int i = -3; i < 4; i++){
            if (i == 0)
                continue;

            if(is_looking_up){
                direccion_x = direccion_x + i*2;
            } else { 
                direccion_y = direccion_y + i*2;
            }

            int bullet_id = municiones * SHOTGUN_ID + i; 
            auto new_bullet = std::make_unique<Bullet>(bullet_id, bullet_pos, direccion_x, direccion_y, map, id_player, alcance, !is_looking_up);
            new_bullet->comenzar_trayectoria();
            projectiles.push_back(std::move(new_bullet));
        }


        municiones--;
        recargando = true;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
        return true;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
        return false;
    }
}

void Shotgun::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

void Shotgun::update_weapon(){

    for(auto it = projectiles.begin(); it != projectiles.end(); ) {
        Projectile* projectile = it->get();

        projectile->update_position();

        if(projectile->should_erase_projectile()) {
            it = projectiles.erase(it);
        } else {
            ++it;
        }
    }

}
