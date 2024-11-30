#include "grenade.h"
#include <iostream>
#include <random>


/* // 6 balas, Alcance: medio (15 tiles (15 x TILE_SIZE = 60))
Grenade::Grenade(int x, int y)
    : Weapon(GRENADE_ID, "Grenade", 60, 0, 1, x, y), activated(false), ticks_counter(0), exploded(false), speed(4, -4) {}
 */
// 6 balas, Alcance: medio (15 tiles (15 x TILE_SIZE = 60))
Grenade::Grenade(int x, int y, int current_ticks)
    : Weapon(GRENADE_ID, "Grenade", 60, 0, 1, x, y), activated(false), ticks_counter(current_ticks), exploded(false), speed(4, -4) {}

bool Grenade::disparar_grenade(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) {
        if (!map && is_looking_up) {
                std::cout << "Position x: " << position_x << ", y: " << position_y
                 << ", looking: " << looking << ", id_player: " << static_cast<uint16_t>(id_player) << std::endl;
        }
        if (!activated) {
                activated = true;
                std::cout << "Se activó la granada" << std::endl;
                return true;
        } else {
                std::cout << "La granada ya está activada" << std::endl;
        }
        return false;
}

void Grenade::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}


// Devuelve true cuando ya no le quedan proyectiles
bool Grenade::update_weapon(int position_x, int position_y, char looking, GameMap* map, char id_player){
        ticks_counter++;

        // Uso la de banana pq hace lo mismo
        Position old_position = position;
        position = map->try_move_grenade(position, speed);
        if (speed.y < 3) {
                speed.y++;
        }
        
        map->clean_projectile_old_position(old_position, 1, 1);
        map->set_projectile_new_position(position, 1, 1, '*');

        if (map->is_throwable_touching_floor(position, 1, 1)) {
                touching_floor = true;
        }

        if (exploded) {
                for(auto it = projectiles.begin(); it != projectiles.end(); ) {
                        Projectile* projectile = it->get();

                        projectile->update_position();

                        if(projectile->should_erase_projectile()) {
                                it = projectiles.erase(it);
                        } else {
                                ++it;
                        }
                }
                if (projectiles.empty()) {
                        return true;
                }
        } else {
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
                                        if (i == 0 && j == 0)
                                                continue;
                                        int direccion_x = i;
                                        int direccion_y = j;

                                        auto new_fragment = std::make_unique<GrenadeFragment>(fragment_count, fragment_position, direccion_x, direccion_y, map, id_player, alcance, true);
                                        new_fragment->comenzar_trayectoria();
                                        projectiles.push_back(std::move(new_fragment));

                                        fragment_count++;
                                }
                        }

                        exploded = true;
                        used = true;
                } else {
                        std::cout << "Pasó un tick, quedan " << ticks_counter - 20 << std::endl;
                }
        }
        return false;
}
