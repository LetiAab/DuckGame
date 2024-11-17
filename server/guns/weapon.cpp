#include "weapon.h"
#include <iostream>

Weapon::Weapon(uint16_t item_id, const std::string& nombre, double alcance, int dispersion, int municiones, int x, int y)
    : Item(item_id,x, y), nombre(nombre), alcance(alcance), dispersion(dispersion), municiones(municiones) {}  // Inicializar posición

void Weapon::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) {
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

        int direccion_x = (looking == LOOKING_RIGHT) ? 6 : -6; //personalizar la velocidad
        int direccion_y = 0;  // La bala se mueve horizonalmente

        int bullet_id = municiones; //el id es el numero de muncion. Inteligente verdad?

        Bullet nueva_bala(bullet_id, bullet_pos, direccion_x, direccion_y, map, id_player, alcance);
        nueva_bala.comenzar_trayectoria();
        bullets.push_back(nueva_bala);

        municiones--;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
    }
}

void Weapon::recargar(int cantidad) {
    municiones += cantidad;
    std::cout << "Se han recargado " << cantidad << " municiones. Ahora tienes " << municiones << " municiones." << std::endl;
}

void Weapon::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

std::string Weapon::getNombre() const {
    return nombre;
}

double Weapon::getAlcance() const {
    return alcance;
}

double Weapon::getDispersion() const {
    return dispersion;
}

int Weapon::getMuniciones() const {
    return municiones;
}

void Weapon::update_weapon(){

    for(auto it = bullets.begin(); it != bullets.end(); ) {
        it->update_position();

        if(it->should_erase_bullet()) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }

}
