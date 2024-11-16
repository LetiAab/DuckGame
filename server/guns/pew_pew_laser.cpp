#include "pew_pew_laser.h"
#include <iostream>

// 12 rafagas, Alcance: 35 a 38 tiles
PewPewLaser::PewPewLaser(const std::string& nombre, double alcance, double dispersion, int municiones, int x, int y)
    : Item(WEAPON_1_ID,x, y), nombre(nombre), alcance(alcance), dispersion(dispersion), municiones(municiones) {}  // Inicializar posición

void PewPewLaser::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) {
    if (municiones > 0) {

        //la bala debe aparecer fuera del pato, o sino se mata a si mismo 
        int direccion_x = (looking == LOOKING_RIGHT) ? DUCK_SIZE_X : -DUCK_SIZE_X;
        int direccion_y = 0;  // La bala se mueve horizonalmente

        int bullet_id = municiones; //el id es el numero de muncion. Inteligente verdad?

        for (int i = -1; i < 2; i++){
                Laser new_laser(bullet_id, position_x, position_y, direccion_x, direccion_y + i, map, id_player, alcance);
                new_laser.comenzar_trayectoria();
                lasers.push_back(new_laser);
        }
        
        municiones--;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
    }
}

void PewPewLaser::recargar(int cantidad) {
    municiones += cantidad;
    std::cout << "Se han recargado " << cantidad << " municiones. Ahora tienes " << municiones << " municiones." << std::endl;
}

void PewPewLaser::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

std::string PewPewLaser::getNombre() const {
    return nombre;
}

double PewPewLaser::getAlcance() const {
    return alcance;
}

double PewPewLaser::getDispersion() const {
    return dispersion;
}

int PewPewLaser::getMuniciones() const {
    return municiones;
}

void PewPewLaser::update_weapon(){

    for(auto it = lasers.begin(); it != lasers.end(); ) {
        it->update_position();

        if(it->should_erase_laser()) {
            it = lasers.erase(it);
        } else {
            ++it;
        }
    }

}
