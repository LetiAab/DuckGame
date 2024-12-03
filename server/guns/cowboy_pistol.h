#ifndef COWBOY_PISTOL_H
#define COWBOY_PISTOL_H

#include <string>
#include <vector>

#include "../../common/constants.h"
#include "../guns/weapon.h"

class Bullet;  // Declaración anticipada de la clase Bullet
class GameMap; // Declaración anticipada de la clase GameMap

class CowboyPistol : public Weapon {  // Herencia de Item
public:

    // Constructor que inicializa la posición
    explicit CowboyPistol(int x = 0, int y = 0);

    // Métodos
    bool disparar_cowboy_pistol(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up);

    bool disparar(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) override {
        return disparar_cowboy_pistol(position_x, position_y, looking, map, id_player, is_looking_up);
    }

    void mostrarInformacion() const;  
    void update_weapon();

    virtual ~CowboyPistol() = default;  // Destructor virtual
};

#endif  // COWBOY_PISTOL_H
