#include "laser_rifle.h"
#include <iostream>

// 10 rayos, Alcance: 30 tiles (30 x TILE_SIZE = 120)
LaserRifle::LaserRifle(int x, int y)
    : Weapon(LASER_RIFLE_ID, "Laser Rifle", 120, 0, 10, x, y) {}

bool LaserRifle::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player,  bool is_looking_up) {
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

        int direccion_x = (looking == LOOKING_RIGHT) ? 6 : -6;
        int direccion_y = 6;  // La dispersion de 45 grados se hace igualando la speed de x y de y

        if(is_looking_up){
            direccion_x = 6;
            direccion_y = -6;
        }

        int laser_id = municiones; //el id es el numero de muncion. Inteligente verdad?

        std::cout << "Creada la bala, a darle vida" << std::endl;
        auto new_laser = std::make_unique<BouncingLaser>(laser_id, laser_pos, direccion_x, direccion_y, map, id_player, alcance, !is_looking_up);
        new_laser->comenzar_trayectoria();
        projectiles.push_back(std::move(new_laser));

        municiones--;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
        return true;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
        return false;
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
