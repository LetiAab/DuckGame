#include "pew_pew_laser.h"
#include <iostream>

// 12 rafagas, Alcance: 35 a 38 tiles (35 x TILE_SIZE = 140)
PewPewLaser::PewPewLaser(int x, int y)
    : Weapon(PEW_PEW_LASER_ID, "Pew Pew Laser", PEW_PEW_LASER_RANGE, 1, PEW_PEW_LASER_AMMUNITION, x, y) {}

bool PewPewLaser::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) {
    if (municiones > 0) {
        //la bala debe aparecer fuera del pato, o sino se mata a si mismo
        int laser_position_x = (looking == LOOKING_RIGHT) ? position_x + DUCK_SIZE_X : position_x -1;
        int laser_position_y = (is_looking_up) ? position_y - DUCK_SIZE_Y / 2 : position_y;

        Position laser_pos(laser_position_x, laser_position_y);
        //si donde debe salir la bala hay una pared, no puedo disparar
        if(map->at(laser_pos)== 'P') {
            std::cout << "No puedo disparar, hay una pared inmediatamente al lado" << std::endl;
            return false;
        }

        int direccion_x = (looking == LOOKING_RIGHT) ? BULLET_SPEED_X : -BULLET_SPEED_X;
        int direccion_y = 0;  // La bala se mueve horizonalmente

        if(is_looking_up){
            direccion_x = 0;
            direccion_y = -BULLET_SPEED_Y;
        }
        for (int i = -1; i < 2; i++){
                int laser_id = municiones * PEW_PEW_LASER_ID + i;

                if(is_looking_up){
                    direccion_x = direccion_x + i;
                } else {
                    direccion_y =  direccion_y + i;
                }

                auto new_laser = std::make_unique<Laser>(laser_id, laser_pos, direccion_x, direccion_y, map, id_player, alcance, !is_looking_up);
                new_laser->comenzar_trayectoria();
                projectiles.push_back(std::move(new_laser));
        }
        
        municiones--;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
        return true;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
        return false;
    }
}

void PewPewLaser::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

void PewPewLaser::update_weapon(){

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
