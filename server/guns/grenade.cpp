#include "grenade.h"
#include <iostream>
#include <random>


// 6 balas, Alcance: medio (15 tiles (15 x TILE_SIZE = 60))
Grenade::Grenade(int x, int y)
    : Weapon(GRENADE_ID, "Grenade", 60, 0, 1, x, y), activated(false), ticks_counter(0) {}

void Grenade::disparar_grenade(int position_x, int position_y, char looking, GameMap* map, char id_player) {
        if (!map) {
                std::cout << "Position x: " << position_x << ", y: " << position_y
                 << ", looking: " << looking << ", id_player: " << static_cast<uint16_t>(id_player) << std::endl;
        }
    if (!activated) {
        activated = true;
        std::cout << "Se activó la granada" << std::endl;
    } else {
        std::cout << "La granada ya está activada" << std::endl;
    }
}

void Grenade::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

std::string Grenade::getNombre() const {
    return nombre;
}

double Grenade::getAlcance() const {
    return alcance;
}

double Grenade::getDispersion() const {
    return dispersion;
}

int Grenade::getMuniciones() const {
    return municiones;
}

void Grenade::update_weapon(int position_x, int position_y, char looking, GameMap* map, char id_player){
        ticks_counter++;

        if (ticks_counter >= 20) {
                // Creo que lo mejor va a ser que explote acá también y mueva los perdigones
                std::cout << "La granada explotó en la mano del pato" << std::endl;
                        
                int fragment_position_x = (looking == LOOKING_RIGHT) ? position_x + DUCK_SIZE_X : position_x -1;
                int fragment_position_y = position_y;

                Position fragment_position(fragment_position_x, fragment_position_y);
/*                 
                //si donde debe salir la bala hay una pared, no puedo disparar
                if(game_map->at( fragment_position)== 'P') {
                std::cout << "No puedo disparar, hay una pared inmediatamente al lado" << std::endl;
                return;
                } 
*/
                int fragment_count = 0;

                // Tiro ocho proyectiles, uno en cada dirección
                for (int i = -1; i < 2; i++) {
                        for (int j = -1; j < 2; j++) {
                                int direccion_x = i;
                                int direccion_y = j;

                                auto new_fragment = std::make_unique<GrenadeFragment>(fragment_count, fragment_position, direccion_x, direccion_y, map, id_player, alcance);
                                new_fragment->comenzar_trayectoria();
                                projectiles.push_back(std::move(new_fragment));

                                fragment_count++;
                        }
                }
        } else {
                std::cout << "Pasó un tick, quedan " << ticks_counter - 20 << std::endl;
        }

}
