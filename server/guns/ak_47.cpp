#include "ak_47.h"
#include <iostream>

// 30 balas, Alcance: 13 tiles
Ak47::Ak47(int x, int y)
    : Weapon(AK_47_ID, "Ak 47", 13, 0, 30, x, y) {}

void Ak47::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) {
    if (municiones > 0) {
        if (dispersion > 5) {
            dispersion = 0;
        }
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

        // Esto no diferencia si los disparos son sequidos o no
        int dispersion_disparo = (dispersion % 2 == 0) ? dispersion : (0-dispersion);

        Bullet new_bullet(bullet_id, bullet_pos, direccion_x, direccion_y + dispersion_disparo, map, id_player, alcance);
        new_bullet.comenzar_trayectoria();
        bullets.push_back(new_bullet);
        
        municiones--;
        dispersion ++;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
    }
}

void Ak47::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

std::string Ak47::getNombre() const {
    return nombre;
}

double Ak47::getAlcance() const {
    return alcance;
}

double Ak47::getDispersion() const {
    return dispersion;
}

int Ak47::getMuniciones() const {
    return municiones;
}

void Ak47::update_weapon(){

    for(auto it = bullets.begin(); it != bullets.end(); ) {
        it->update_position();

        if(it->should_erase_bullet()) {
              it = bullets.erase(it);
        } else {
            ++it;
        }
    }

}
