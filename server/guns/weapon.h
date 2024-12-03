#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <vector>

#include "../game/item.h"
#include "common/constants.h"
#include "../game/game_map.h"
#include "bullet.h"

class Weapon : public Item {
public:
    std::vector<std::unique_ptr<Projectile>> projectiles;

    // Constructor que inicializa la posición
    Weapon(uint16_t item_id, const std::string& nombre, double alcance, int dispersion, int municiones, int x = 0, int y = 0);

    // Métodos
    void mostrarInformacion() const;  
    void update_weapon();

    int getMuniciones() const;
    
    bool isUsed();
    void setUsed(bool new_used);

    bool get_throwed_position_message(Message& throwed_message);

    virtual bool disparar(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) = 0;
    virtual ~Weapon() = default;  // Destructor virtual

protected:
    std::string nombre;
    double alcance;
    int dispersion;
    int municiones;
    bool used;
    bool touching_floor;
};

#endif  // WEAPON_H
