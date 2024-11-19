#include "magnum.h"


// 6 balas, Alcance: 20 tiles
Magnum::Magnum(int x, int y)
    : Weapon(MAGNUM_ID, "Magnum", 20, 0, 6, x, y) {}

void Magnum::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) {
    if (municiones > 0) {
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

        int bullet_id = municiones; //el id es el numero de muncion. Inteligente verdad?

        // Generación aleatoria de la dispersión (sacar si saca mucho cómputo)
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> distrib(-3, 3);
        int random_dispersion = distrib(gen);

        auto new_bullet = std::make_unique<Bullet>(bullet_id, bullet_pos, direccion_x, direccion_y + random_dispersion, map, id_player, alcance);
        new_bullet->comenzar_trayectoria();
        projectiles.push_back(std::move(new_bullet));
        
        municiones--;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
    }
}

void Magnum::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

std::string Magnum::getNombre() const {
    return nombre;
}

double Magnum::getAlcance() const {
    return alcance;
}

double Magnum::getDispersion() const {
    return dispersion;
}

int Magnum::getMuniciones() const {
    return municiones;
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
