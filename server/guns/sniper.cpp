#include "sniper.h"
#include <iostream>
#include <random>


// 3 balas, Alcance: 64 tiles
Sniper::Sniper(int x, int y)
    : Weapon(SNIPER_ID, "Sniper", 64, 0, 3, x, y) {}  // Inicializar posición

void Sniper::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) {
    if (municiones > 0) {
        //la bala debe aparecer fuera del pato, o sino se mata a si mismo
        int bullet_position_x = (looking == LOOKING_RIGHT) ? position_x + DUCK_SIZE_X : position_x -1;
        int bullet_position_y = position_y;

        Position bullet_pos(bullet_position_x, bullet_position_y);
        //si donde debe salir la bala hay una pared, no puedo disparar
        std::cout << "Antes del map->at() \n";
        if(map->at(bullet_pos)== 'P') {
            std::cout << "No puedo disparar, hay una pared inmediatamente al lado" << std::endl;
            return;
        }

        int direccion_x = (looking == LOOKING_RIGHT) ? 6 : -6;
        int direccion_y = 0;  // La bala se mueve horizonalmente

        int bullet_id = municiones; //el id es el numero de muncion. Inteligente verdad?

        std::cout << "Creando la bala \n";
        Bullet new_bullet(bullet_id, bullet_pos, direccion_x, direccion_y, map, id_player, alcance);
        new_bullet.comenzar_trayectoria();
        bullets.push_back(new_bullet);
        
        municiones--;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
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

    for(auto it = bullets.begin(); it != bullets.end(); ) {
        it->update_position();

        if(it->should_erase_bullet()) {
              it = bullets.erase(it);
        } else {
            ++it;
        }
    }

}
