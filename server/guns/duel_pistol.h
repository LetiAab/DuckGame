#ifndef DUEL_PISTOL_H
#define DUEL_PISTOL_H

#include <string>
#include <vector>

#include "bullet.h"
#include "../game/game_map.h"
//#include "common/constants.h"
#include "weapon.h"

class Bullet;  // Declaración anticipada de la clase Bullet
class GameMap; // Declaración anticipada de la clase GameMap

class DuelPistol : public Weapon {  // Herencia de Item
public:
    // Could be just an object
    std::vector<Bullet> bullets;

    // Constructor que inicializa la posición
    DuelPistol(int x = 0, int y = 0);

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

#endif  // DUEL_PISTOL_H