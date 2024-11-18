#ifndef COWBOY_PISTOL_H
#define COWBOY_PISTOL_H

#include <string>
#include <vector>

#include "common/constants.h"
#include "../guns/weapon.h"

class Bullet;  // Declaración anticipada de la clase Bullet
class GameMap; // Declaración anticipada de la clase GameMap

class CowboyPistol : public Weapon {  // Herencia de Item
public:
    // Could be just an object
    std::vector<Bullet> bullets;

    // Constructor que inicializa la posición
    CowboyPistol(int x = 0, int y = 0);

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

#endif  // COWBOY_PISTOL_H
