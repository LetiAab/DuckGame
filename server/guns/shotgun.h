#ifndef SHOTGUN_H
#define SHOTGUN_H

#include <string>
#include <vector>

#include "common/constants.h"
#include "../guns/weapon.h"

class Bullet;  // Declaración anticipada de la clase Bullet
class GameMap; // Declaración anticipada de la clase GameMap

class Shotgun : public Weapon {  // Herencia de Item
public:

    // Constructor que inicializa la posición
    Shotgun(int x = 0, int y = 0);

    // Métodos
    bool disparar(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) override;

    void mostrarInformacion() const;  
    void update_weapon();
    
private:
    bool recargando;
};

#endif  // SHOTGUN_H
