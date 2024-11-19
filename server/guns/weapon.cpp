#include "weapon.h"
#include <iostream>

Weapon::Weapon(uint16_t item_id, const std::string& nombre, double alcance, int dispersion, int municiones, int x, int y)
    : Item(item_id,x, y), nombre(nombre), alcance(alcance), dispersion(dispersion), municiones(municiones) {}  // Inicializar posición


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
