#include "banana.h"
#include <iostream>
#include <random>


// 6 balas, Alcance: medio (15 tiles (15 x TILE_SIZE = 60))
Banana::Banana(int x, int y)
    : Weapon(BANANA_ID, "Banana", 60, 0, 1, x, y), peeled(false), speed(4, -4), pisada(false) {}

void Banana::disparar_banana(int position_x, int position_y, char looking, GameMap* map, char id_player) {
        if (!map) {
                std::cout << "Position x: " << position_x << ", y: " << position_y
                 << ", looking: " << looking << ", id_player: " << static_cast<uint16_t>(id_player) << std::endl;
        }
    if (!peeled) {
        peeled = true;
        std::cout << "Se peló la banana" << std::endl;
    } else {
        std::cout << "La banana ya está pelada" << std::endl;
    }
}

void Banana::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

std::string Banana::getNombre() const {
    return nombre;
}

double Banana::getAlcance() const {
    return alcance;
}

double Banana::getDispersion() const {
    return dispersion;
}

int Banana::getMuniciones() const {
    return municiones;
}



void Banana::update_weapon(GameMap& map){
        if(pisada){
            //si empacte con algo debo eliminar la bala luego de mandar el mensaje
            map.clean_projectile_old_position(position, 1, 1);
            // should_erase = true;
        }

        // Mover a la banana en el mapa. Mi idea es, tiene una velocidad de -4 en y, y le voy sumando 1 cada vez que pasa una iteración
        Position old_position = position;
        position = map.try_move_banana(position, speed, pisada);
        if (speed.y < 3) {
                speed.y++;
        }

        if (pisada) {
            used = true;
        }
        
        map.clean_projectile_old_position(old_position, 1, 1);
        map.set_projectile_new_position(position, 1, 1, 'B');

        if (map.is_throwable_touching_floor(position, 1, 1)) {
                touching_floor = true;
        }
}
