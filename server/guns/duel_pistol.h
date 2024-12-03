#ifndef DUEL_PISTOL_H
#define DUEL_PISTOL_H

#include <string>
#include <vector>

#include "bullet.h"
#include "../game/game_map.h"
//#include "../common/constants.h"
#include "weapon.h"

class Bullet;  // Declaración anticipada de la clase Bullet
class GameMap; // Declaración anticipada de la clase GameMap

class DuelPistol : public Weapon {  // Herencia de Item
public:

    // Constructor que inicializa la posición
    DuelPistol(int x = 0, int y = 0);

    // Métodos
    bool disparar(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) override;

    void mostrarInformacion() const;  
    void update_weapon();
};

#endif  // DUEL_PISTOL_H
