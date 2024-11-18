#include "duel_pistol.h"
#include <iostream>
#include <random>


// 1 bala, Alcance: 5 tiles
DuelPistol::DuelPistol(int x, int y)
    : Weapon(DUEL_PISTOL_ID, "Duel Pistol", 5, 0, 1, x, y) {}

void DuelPistol::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) {
    if (municiones > 0) {
        //la bala debe aparecer fuera del pato, o sino se mata a si mismo
        int bullet_position_x = (looking == LOOKING_RIGHT) ? position_x + DUCK_SIZE_X : position_x -1;
        int bullet_position_y = position_y;

        Position bullet_pos(bullet_position_x, bullet_position_y);
        //si donde debe salir la bala hay una pared, no puedo disparar
        if(map->at(bullet_pos)== 'P') {
            std::cout << "No puedo disparar, hay una pared inmediatamente al lado" << std::endl;
            return;
        }

        int direccion_x = (looking == LOOKING_RIGHT) ? 6 : -6;
        int direccion_y = 0;  // La bala se mueve horizonalmente

        int bullet_id = municiones; //el id es el numero de muncion. Inteligente verdad?

        // Generación aleatoria de la dispersión (sacar si saca mucho cómputo)
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> distrib(-3, 3);
        int random_dispersion = distrib(gen);

        Bullet new_bullet(bullet_id, bullet_pos, direccion_x, direccion_y + random_dispersion, map, id_player, alcance);
        new_bullet.comenzar_trayectoria();
        bullets.push_back(new_bullet);
        
        municiones--;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
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

    for(auto it = bullets.begin(); it != bullets.end(); ) {
        it->update_position();

        if(it->should_erase_bullet()) {
              it = bullets.erase(it);
        } else {
            ++it;
        }
    }

}