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

    // Getters
    std::string getNombre() const;
    double getAlcance() const;
    double getDispersion() const;
    int getMuniciones() const;

    bool get_throwed_position_message(Message& throwed_message);

    virtual void disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) = 0;
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
