#include "laser_rifle.h"
#include <iostream>

#define LASER_RIFLE_NAME "Laser Rifle"
#define LASER_RIFLE_RANGE 30
#define LASER_RIFLE_DISPERSION 5  // Variación del ángulo en disparos consecutivos
#define LASER_RIFLE_INITIAL_AMMO 10
#define LASER_RIFLE_REBOUND_LIMIT 5

// 10 rayos, Alcance: 30 tiles
LaserRifle::LaserRifle(const std::string& nombre, double alcance, double dispersion, int municiones, int x, int y)
    : Item(WEAPON_1_ID,x, y), nombre(nombre), alcance(alcance), dispersion(dispersion), municiones(municiones) {}  // Inicializar posición

void LaserRifle::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) {
    if (municiones > 0) {
        //la bala debe aparecer fuera del pato, o sino se mata a si mismo
        int laser_position_x = (looking == LOOKING_RIGHT) ? position_x + DUCK_SIZE_X : position_x -1;
        int laser_position_y = position_y;

        Position laser_pos(laser_position_x, laser_position_y);
        //si donde debe salir la bala hay una pared, no puedo disparar
        if(map->at(laser_pos)== 'P') {
            std::cout << "No puedo disparar, hay una pared inmediatamente al lado" << std::endl;
            return;
        }

        int direccion_x = (looking == LOOKING_RIGHT) ? 6 : -6;
        int direccion_y = 6;  // La dispersion de 45 grados se hace igualando la speed de x y de y

        int laser_id = municiones; //el id es el numero de muncion. Inteligente verdad?

        BouncingLaser new_laser(laser_id, laser_pos, direccion_x, direccion_y, map, id_player, alcance);
        new_laser.comenzar_trayectoria();
        lasers.push_back(new_laser);
        
        municiones--;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
    }
}

void LaserRifle::recargar(int cantidad) {
    municiones += cantidad;
    std::cout << "Se han recargado " << cantidad << " municiones. Ahora tienes " << municiones << " municiones." << std::endl;
}

void LaserRifle::mostrarInformacion() const {
    std::cout << "Arma: " << nombre << std::endl;
    std::cout << "Alcance: " << alcance << " metros" << std::endl;
    std::cout << "Dispersion: " << dispersion << std::endl;
    std::cout << "Municiones: " << municiones << std::endl;
}

std::string LaserRifle::getNombre() const {
    return nombre;
}

double LaserRifle::getAlcance() const {
    return alcance;
}

double LaserRifle::getDispersion() const {
    return dispersion;
}

int LaserRifle::getMuniciones() const {
    return municiones;
}

void LaserRifle::update_weapon(){

    for(auto it = lasers.begin(); it != lasers.end(); ) {
        it->update_position();

        if(it->should_erase_laser()) {
            it = lasers.erase(it);
        } else {
            ++it;
        }
    }

}


/* #include "laser_rifle.h"
#include <iostream>
#include <cmath>

#define LASER_RIFLE_NAME "Laser Rifle"
#define LASER_RIFLE_RANGE 30
#define LASER_RIFLE_DISPERSION 5  // Variación del ángulo en disparos consecutivos
#define LASER_RIFLE_INITIAL_AMMO 10
#define LASER_RIFLE_REBOUND_LIMIT 5

LaserRifle::LaserRifle(int x, int y)
    : Weapon(LASER_RIFLE_NAME, LASER_RIFLE_RANGE, LASER_RIFLE_DISPERSION, LASER_RIFLE_INITIAL_AMMO, x, y),
      current_angle(45.0),  // Ángulo inicial de 45 grados
      rebound_limit(LASER_RIFLE_REBOUND_LIMIT) {}

void LaserRifle::disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) {
    if (municiones > 0) {
        int bullet_position_x = (looking == LOOKING_RIGHT) ? position_x + DUCK_SIZE_X : position_x - 1;
        int bullet_position_y = position_y;

        Position bullet_pos(bullet_position_x, bullet_position_y);

        if (map->at(bullet_pos) == 'P') {
            std::cout << "No puedo disparar, hay una pared inmediatamente al lado." << std::endl;
            return;
        }

        int direccion_x = (looking == LOOKING_RIGHT) ? 6 : -6;
        int direccion_y = 6;  // Inclinación inicial de 45 grados hacia abajo

        int laser_id = municiones;

        // Crear un nuevo láser con rebotes
        BouncingLaser nuevo_laser(laser_id, bullet_pos, direccion_x, direccion_y, map, id_player, alcance, 3);  // Hasta 3 rebotes
        nuevo_laser.comenzar_trayectoria();
        lasers.push_back(nuevo_laser);

        municiones--;
        std::cout << "Disparo realizado. Quedan " << municiones << " municiones." << std::endl;
    } else {
        std::cout << "No hay municiones disponibles." << std::endl;
    }
}


void LaserRifle::update_weapon() {
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->update_position();
        if (it->should_erase_bullet()) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
}
 */