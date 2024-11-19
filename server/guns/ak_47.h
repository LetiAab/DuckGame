#ifndef AK_47_H
#define AK_47_H

#include <string>
#include <vector>

#include "bullet.h"
#include "../game/game_map.h"
#include "common/constants.h"
#include "weapon.h"

class Bullet;  // Declaración anticipada de la clase Bullet
class GameMap; // Declaración anticipada de la clase GameMap

class Ak47 : public Weapon {  // Herencia de Item
public:
    // Constructor que inicializa la posición
    Ak47(int x = 0, int y = 0);

    // Métodos
    void disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) override;

    void mostrarInformacion() const;  
    void update_weapon();

    // Getters
    std::string getNombre() const;
    double getAlcance() const;
    double getDispersion() const;
    int getMuniciones() const;
};

#endif  // AK_47_H
