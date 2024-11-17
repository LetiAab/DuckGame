#ifndef PEW_PEW_LASER_H
#define PEW_PEW_LASER_H

#include <string>
#include <vector>

#include "laser.h"
#include "common/constants.h"
#include "../guns/weapon.h"

class Laser;  // Declaración anticipada de la clase Bullet
class GameMap; // Declaración anticipada de la clase GameMap

class PewPewLaser : public Weapon {  // Herencia de Item
public:
    std::vector<Laser> lasers;

    // Constructor que inicializa la posición
    PewPewLaser(int x = 0, int y = 0);

    // Métodos
    void disparar(int position_x, int position_y, char looking, GameMap* map, char id_player);

    void mostrarInformacion() const;  
    void update_weapon();

    // Getters
    std::string getNombre() const;
    double getAlcance() const;
    double getDispersion() const;
    int getMuniciones() const;
};

#endif  // PEW_PEW_LASER_H
