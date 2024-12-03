#ifndef COWBOY_PISTOL_H
#define COWBOY_PISTOL_H

#include <string>
#include <vector>

#include "../../common/constants.h"
#include "../guns/weapon.h"

class Bullet;
class GameMap;

class CowboyPistol : public Weapon {
public:

    explicit CowboyPistol(int x = 0, int y = 0);

    // Métodos
    bool disparar(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) override;

    void mostrarInformacion() const;
    void update_weapon();

    virtual ~CowboyPistol() = default;
};

#endif  // COWBOY_PISTOL_H
