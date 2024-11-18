#include "shotgun.h"
#include <iostream>

// 2 disparos, Alcance: 7 a 9 tiles
Shotgun::Shotgun(int x, int y)
    : Weapon(SHOTGUN_ID, "Shotgun", 7, 0, 2, x, y) {}

void Shotgun::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) {
    if (municiones > 0) {
        if (recargando) {
                recargando = false;
                return;
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

        int bullet_id = municiones; //el id es el numero de municion. Inteligente verdad?

        for (int i = -3; i < 4; i++){
                if (i == 0)
                        continue;
                Bullet new_bullet(bullet_id, bullet_pos, direccion_x, direccion_y + i, map, id_player, alcance);
                new_bullet.comenzar_trayectoria();
                bullets.push_back(new_bullet);
        }
        
        municiones--;
        recargando = true;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
    }
}

void Shotgun::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

std::string Shotgun::getNombre() const {
    return nombre;
}

double Shotgun::getAlcance() const {
    return alcance;
}

double Shotgun::getDispersion() const {
    return dispersion;
}

int Shotgun::getMuniciones() const {
    return municiones;
}

void Shotgun::update_weapon(){

    for(auto it = bullets.begin(); it != bullets.end(); ) {
        it->update_position();

        if(it->should_erase_bullet()) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }

}
