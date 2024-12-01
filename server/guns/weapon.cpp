#include "weapon.h"
#include <iostream>

Weapon::Weapon(uint16_t item_id, const std::string& nombre, double alcance, int dispersion, int municiones, int x, int y)
    : Item(item_id,x, y), nombre(nombre), alcance(alcance), dispersion(dispersion), municiones(municiones), used(false), touching_floor(false) {}  // Inicializar posición


void Weapon::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

int Weapon::getMuniciones() const {
    return municiones;
}

bool Weapon::get_throwed_position_message(Message& msg) {

    if (used) {
        return false;
    }
    //std::cout << "LE PASO EL SPAWN CON x: " << position.x << " y: " << position.y << "\n";

    msg.type = THROWABLE_ITEM;
    msg.item_x = position.x;
    msg.item_y = position.y;
    msg.item_id = item_id;
    msg.item_used = used;
    msg.item_touching_floor = touching_floor;

    return true;
}

void Weapon::update_weapon(){

    for(auto it = projectiles.begin(); it != projectiles.end(); ) {
        Projectile* projectile = it->get();

        projectile->update_position();

        //std::cout << "Despues de actualizar una bala \n";

        if(projectile->should_erase_projectile()) {
            it = projectiles.erase(it);
        } else {
            ++it;
        }
        //std::cout << "Despues de chequear que debería borrar una bala \n";
    }

}

bool Weapon::isUsed() {
    return used;
}

void Weapon::setUsed(bool new_used) {
    used = new_used;
}
