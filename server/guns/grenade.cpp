#include "grenade.h"
#include <iostream>
#include <random>


/* // 6 balas, Alcance: medio (15 tiles (15 x TILE_SIZE = 60))
Grenade::Grenade(int x, int y)
    : Weapon(GRENADE_ID, "Grenade", 60, 0, 1, x, y), activated(false), ticks_counter(0), exploded(false), speed(4, -4) {}
 */
// 6 balas, Alcance: medio (15 tiles (15 x TILE_SIZE = 60))
Grenade::Grenade(int x, int y, int current_ticks, Position speed)
    : Weapon(GRENADE_ID, "Grenade", 1000, 0, 1, x, y), activated(false), ticks_counter(current_ticks), exploded(false), speed(speed) {}

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

void Grenade::simulate_movement(GameMap* map) {
        // Uso la de banana pq hace lo mismo
        Position old_position = position;
        bool hit_void = false;
        position = map->try_move_grenade(position, speed, hit_void);
        if (speed.y < 3) {
                speed.y++;
        }

        if (hit_void) {
            map->clean_projectile_old_position(old_position, 1, 1);
            used = true;
            return;
        }
        
        map->clean_projectile_old_position(old_position, 1, 1);
        map->set_projectile_new_position(position, 1, 1, '*');

        if (map->is_throwable_touching_floor(position, 1, 1)) {
                touching_floor = true;
        }
}

int Grenade::getCurrentTicks() {
        return ticks_counter;
}


// Devuelve true cuando ya no le quedan proyectiles
bool Grenade::update_weapon(int position_x, int position_y, char looking, GameMap* map, char id_player){
        ticks_counter++;

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
                simulate_movement(map);

                if (ticks_counter >= 20) {
                        // Creo que lo mejor va a ser que explote acá también y mueva los perdigones
                        std::cout << "La granada explotó" << std::endl;
                                
                        int fragment_position_x = (looking == LOOKING_RIGHT) ? position_x + DUCK_SIZE_X : position_x -1;
                        int fragment_position_y = position_y;

                        Position fragment_position(fragment_position_x, fragment_position_y);

                        int fragment_count = 0;

                        // Tiro ocho proyectiles, uno en cada dirección
                        for (int i = -1; i < 2; i++) {
                                for (int j = -1; j < 2; j++) {
                                        if (i == 0 && j == 0)
                                                continue;
                                        int direccion_x = i;
                                        int direccion_y = j;

                                        auto new_fragment = std::make_unique<GrenadeFragment>(fragment_count, fragment_position, direccion_x, direccion_y, map, id_player, 20, true);
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

void Grenade::prepare_drop(char duck_looking) {
    if (duck_looking == LOOKING_LEFT) {
        speed.x = -3;
    } else {
        speed.x = 3;
    }
}
