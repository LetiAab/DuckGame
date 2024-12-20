#include "banana.h"
#include <iostream>
#include <random>

#define BANANA 'b'


// 6 balas, Alcance: medio (15 tiles (15 x TILE_SIZE = 60))
Banana::Banana(int x, int y, Position speed)
    : Weapon(BANANA_ID, "Banana", 1000, 0, 1, x, y), peeled(false), speed(speed), pisada(false) {}

bool Banana::disparar_banana(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) {
        if (!map && is_looking_up) {
                std::cout << "Position x: " << position_x << ", y: " << position_y
                 << ", looking: " << looking << ", id_player: " << static_cast<uint16_t>(id_player) << std::endl;
        }
    if (!peeled) {
        peeled = true;
        std::cout << "Se peló la banana" << std::endl;
        return true;
    } else {
        std::cout << "La banana ya está pelada" << std::endl;
    }
    return false;
}

void Banana::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

void Banana::update_weapon(GameMap& map){
        if(pisada){
            //si empacte con algo debo eliminar la bala luego de mandar el mensaje
            map.clean_projectile_old_position(position, 1, 1);
            used = true;
        }

        // Mover a la banana en el mapa. Mi idea es, tiene una velocidad de -2 en y, y le voy sumando 1 cada vez que pasa una iteración
        Position old_position = position;
        bool hit_void = false;
        position = map.try_move_banana(position, speed, hit_void);
        if (speed.y < 3) {
                speed.y++;
        }

        if (hit_void) {
            map.clean_projectile_old_position(old_position, 1, 1);
            used = true;
            return;
        }
        
        map.clean_projectile_old_position(old_position, 1, 1);
        map.set_projectile_new_position(position, 1, 1, BANANA);

        if (map.is_throwable_touching_floor(position, 1, 1)) {
                touching_floor = true;
        }
}

void Banana::prepare_drop(char duck_looking) {
    if (duck_looking == LOOKING_LEFT) {
        speed.x = -3;
    } else {
        speed.x = 3;
    }
}

