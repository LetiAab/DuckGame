#ifndef LASER_RIFLE_H
#define LASER_RIFLE_H

#include <string>
#include <vector>

#include "bouncing_laser.h"
#include "common/constants.h"
#include "common/position.h"
#include "../guns/weapon.h"

class GameMap; // Declaración anticipada de la clase GameMap 

class LaserRifle : public Weapon {  // Herencia de Item
public:
    // Constructor que inicializa la posición
    LaserRifle(int x = 0, int y = 0);

    // Métodos
    bool disparar(int position_x, int position_y, char looking, GameMap* map, char id_player,  bool is_looking_up) override;

    void mostrarInformacion() const;  
    void update_weapon();

    // Getters
    std::string getNombre() const;
    double getAlcance() const;
    double getDispersion() const;
    int getMuniciones() const;
};

#endif  // LASER_RIFLE_H
