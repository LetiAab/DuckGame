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

    for(auto it = projectiles.begin(); it != projectiles.end(); ) {
        Projectile* projectile = it->get();

        projectile->update_position();

        std::cout << "Despues de actualizar una bala \n";

        if(projectile->should_erase_projectile()) {
            it = projectiles.erase(it);
        } else {
            ++it;
        }
        std::cout << "Despues de chequear que debería borrar una bala \n";
    }

}
