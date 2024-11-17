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
    std::vector<Bullet> bullets;

    // Constructor que inicializa la posición
    Weapon(uint16_t item_id, const std::string& nombre, double alcance, int dispersion, int municiones, int x = 0, int y = 0);

    // Métodos
    void disparar(int position_x, int position_y, char looking, GameMap* map, char id_player);
    void recargar(int cantidad);  

    void mostrarInformacion() const;  
    void update_weapon();

    // Getters
    std::string getNombre() const;
    double getAlcance() const;
    double getDispersion() const;
    int getMuniciones() const;

protected:
    std::string nombre;
    double alcance;
    int dispersion;
    int municiones;
};

#endif  // WEAPON_H
