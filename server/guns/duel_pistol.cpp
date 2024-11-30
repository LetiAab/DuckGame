#include "duel_pistol.h"
#include <iostream>
#include <random>


// 1 bala, Alcance: 5 tiles
DuelPistol::DuelPistol(int x, int y)
    : Weapon(DUEL_PISTOL_ID, "Duel Pistol", 5, 0, 1, x, y) {}

bool DuelPistol::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) {
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

         // Generación aleatoria de la dispersión (sacar si saca mucho cómputo)
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> distrib(-3, 3);
        int random_dispersion = distrib(gen);

        int direccion_x = (looking == LOOKING_RIGHT) ? 6 : -6;
        int direccion_y = 0 + random_dispersion;  // La bala se mueve horizonalmente

        if(is_looking_up){
            direccion_x = 0 + random_dispersion;
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

void DuelPistol::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

std::string DuelPistol::getNombre() const {
    return nombre;
}

double DuelPistol::getAlcance() const {
    return alcance;
}

double DuelPistol::getDispersion() const {
    return dispersion;
}

int DuelPistol::getMuniciones() const {
    return municiones;
}

void DuelPistol::update_weapon(){

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
