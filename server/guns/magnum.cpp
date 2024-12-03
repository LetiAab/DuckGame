#include "magnum.h"


// 6 balas, Alcance: 20 tiles (20 x TILE_SIZE = 80)
Magnum::Magnum(int x, int y)
    : Weapon(MAGNUM_ID, "Magnum", MAGNUM_RANGE, 0, MAGNUM_AMMUNITION, x, y) {}

bool Magnum::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) {
    if (municiones > 0) {
        //la bala debe aparecer fuera del pato, o sino se mata a si mismo
        int bullet_position_x = (looking == LOOKING_RIGHT) ? position_x + DUCK_SIZE_X : position_x -1;
        int bullet_position_y = (is_looking_up) ? position_y - DUCK_SIZE_Y / 2 : position_y;

        Position bullet_pos(bullet_position_x, bullet_position_y);
        //si donde debe salir la bala hay una pared, no puedo disparar
        if(map->at(bullet_pos)== 'P') {
            std::cout << "No puedo disparar, hay una pared inmediatamente al lado" << std::endl;
            return false;
        }

        // Generación aleatoria de la dispersión (sacar si saca mucho cómputo)
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> distrib(-3, 3);
        int random_dispersion = distrib(gen);
 
        int direccion_x = (looking == LOOKING_RIGHT) ? BULLET_SPEED_X : -BULLET_SPEED_X;
        int direccion_y = 0 + random_dispersion;  // La bala se mueve horizonalmente

        if(is_looking_up){
            direccion_x = 0 + random_dispersion;
            direccion_y = -BULLET_SPEED_Y;

        }

        int bullet_id = municiones; //el id es el numero de muncion. Inteligente verdad?



        auto new_bullet = std::make_unique<Bullet>(bullet_id, bullet_pos, direccion_x, direccion_y, map, id_player, alcance, !is_looking_up);
        new_bullet->comenzar_trayectoria();
        projectiles.push_back(std::move(new_bullet));
        
        municiones--;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
        return true;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
        return false;
    }
}

void Magnum::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

void Magnum::update_weapon(){

    for(auto it = projectiles.begin(); it != projectiles.end(); ) {
        Projectile* projectile = it->get();

        projectile->update_position();

        if(projectile->should_erase_projectile()) {
              it = projectiles.erase(it);
        } else {
            ++it;
        }
    }

}
