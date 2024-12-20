#ifndef MAGNUM_H
#define MAGNUM_H

#include <string>
#include <vector>
#include <iostream>
#include <random>

#include "common/constants.h"
#include "../guns/weapon.h"


class Bullet;  // Declaración anticipada de la clase Bullet
class GameMap; // Declaración anticipada de la clase GameMap

class Magnum : public Weapon {  // Herencia de Item
public:

    // Constructor que inicializa la posición
    Magnum(int x = 0, int y = 0);

    // Métodos
    bool disparar(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) override;

    void mostrarInformacion() const;  
    void update_weapon();
};

#endif  // MAGNUM_H
