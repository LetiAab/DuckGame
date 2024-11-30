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
    // Constructor que inicializa la posición
    PewPewLaser(int x = 0, int y = 0);

    // Métodos
    bool disparar(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) override;

    void mostrarInformacion() const;  
    void update_weapon();
};

#endif  // PEW_PEW_LASER_H
