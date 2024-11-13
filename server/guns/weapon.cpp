#include "weapon.h"
#include <iostream>

Weapon::Weapon(const std::string& nombre, double alcance, double dispersion, int municiones, int x, int y)
    : Item(x, y), nombre(nombre), alcance(alcance), dispersion(dispersion), municiones(municiones) {}  // Inicializar posición

void Weapon::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) {
    if (municiones > 0) {
        int direccion_x = (looking == LOOKING_RIGHT) ? 5 : -5;
        int direccion_y = 0;

        int bullet_id = municiones;

        Bullet nueva_bala(bullet_id, position_x, position_y + 1, direccion_x, direccion_y, map, id_player, alcance);
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
