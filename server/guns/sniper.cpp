#include "sniper.h"
#include <iostream>
#include <random>


// 3 balas, Alcance: 64 tiles (64 x TILE_SIZE = 256)
Sniper::Sniper(int x, int y)
    : Weapon(SNIPER_ID, "Sniper", 256, 0, 3, x, y) {}  // Inicializar posición

bool Sniper::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) {
    if (municiones > 0) {
        //la bala debe aparecer fuera del pato, o sino se mata a si mismo
        int bullet_position_x = (looking == LOOKING_RIGHT) ? position_x + DUCK_SIZE_X : position_x -1;
        int bullet_position_y = (is_looking_up) ? position_y - DUCK_SIZE_Y / 2 : position_y;

        Position bullet_pos(bullet_position_x, bullet_position_y);
        //si donde debe salir la bala hay una pared, no puedo disparar
        if(map->at(bullet_pos)== 'P') {
            std::cout << "No puedo disparar, hay una pared inmediatamente al lado" << std::endl;
            return false;
        }

        int direccion_x = (looking == LOOKING_RIGHT) ? 6 : -6;
        int direccion_y = 0;  // La bala se mueve horizonalmente
        
        if(is_looking_up){
            direccion_x = 0;
            direccion_y = -6;
        }

        int bullet_id = municiones; //el id es el numero de muncion. Inteligente verdad?

        auto new_bullet = std::make_unique<Bullet>(bullet_id, bullet_pos, direccion_x, direccion_y, map, id_player, alcance, !is_looking_up);
        new_bullet->comenzar_trayectoria();
        projectiles.push_back(std::move(new_bullet));
        
        municiones--;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
        return true;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
        return false;
    }
}

void Sniper::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

std::string Sniper::getNombre() const {
    return nombre;
}

double Sniper::getAlcance() const {
    return alcance;
}

double Sniper::getDispersion() const {
    return dispersion;
}

int Sniper::getMuniciones() const {
    return municiones;
}

void Sniper::update_weapon(){

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
