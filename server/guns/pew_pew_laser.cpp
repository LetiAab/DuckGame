#include "pew_pew_laser.h"
#include <iostream>

// 12 rafagas, Alcance: 35 a 38 tiles (35 x TILE_SIZE = 140)
PewPewLaser::PewPewLaser(int x, int y)
    : Weapon(PEW_PEW_LASER_ID, "Pew Pew Laser", 140, 1, 12, x, y) {}

void PewPewLaser::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) {
    if (municiones > 0) {
        //la bala debe aparecer fuera del pato, o sino se mata a si mismo
        int laser_position_x = (looking == LOOKING_RIGHT) ? position_x + DUCK_SIZE_X : position_x -1;
        int laser_position_y = position_y;

        Position laser_pos(laser_position_x, laser_position_y);
        //si donde debe salir la bala hay una pared, no puedo disparar
        if(map->at(laser_pos)== 'P') {
            std::cout << "No puedo disparar, hay una pared inmediatamente al lado" << std::endl;
            return;
        }

        int direccion_x = (looking == LOOKING_RIGHT) ? 6 : -6;
        int direccion_y = 0;  // La bala se mueve horizonalmente

        for (int i = -1; i < 2; i++){
                int laser_id = municiones * PEW_PEW_LASER_ID + i; 

                auto new_laser = std::make_unique<Laser>(laser_id, laser_pos, direccion_x, direccion_y + i, map, id_player, alcance);
                new_laser->comenzar_trayectoria();
                projectiles.push_back(std::move(new_laser));
        }
        
        municiones--;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
    }
}

void PewPewLaser::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

std::string PewPewLaser::getNombre() const {
    return nombre;
}

double PewPewLaser::getAlcance() const {
    return alcance;
}

double PewPewLaser::getDispersion() const {
    return dispersion;
}

int PewPewLaser::getMuniciones() const {
    return municiones;
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
