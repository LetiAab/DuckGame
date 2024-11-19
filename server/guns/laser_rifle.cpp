#include "laser_rifle.h"
#include <iostream>

// 10 rayos, Alcance: 30 tiles
LaserRifle::LaserRifle(int x, int y)
    : Weapon(LASER_RIFLE_ID, "Laser Rifle", 30, 0, 10, x, y) {}

void LaserRifle::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) {
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
        int direccion_y = 6;  // La dispersion de 45 grados se hace igualando la speed de x y de y

        int laser_id = municiones; //el id es el numero de muncion. Inteligente verdad?

        auto new_laser = std::make_unique<BouncingLaser>(laser_id, laser_pos, direccion_x, direccion_y, map, id_player, alcance);
        new_laser->comenzar_trayectoria();
        projectiles.push_back(std::move(new_laser));
        
        municiones--;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
    }
}

void LaserRifle::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

std::string LaserRifle::getNombre() const {
    return nombre;
}

double LaserRifle::getAlcance() const {
    return alcance;
}

double LaserRifle::getDispersion() const {
    return dispersion;
}

int LaserRifle::getMuniciones() const {
    return municiones;
}

void LaserRifle::update_weapon(){

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
