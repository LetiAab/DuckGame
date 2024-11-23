#ifndef GRENADE_H
#define GRENADE_H

#include <string>
#include <vector>

#include "grenade_fragment.h"
#include "common/constants.h"
#include "../guns/weapon.h"

class GameMap; // Declaración anticipada de la clase GameMap

class Grenade : public Weapon {  // Herencia de Item
public:
    bool activated;
    int ticks_counter;

    // Constructor que inicializa la posición
    explicit Grenade(int x = 0, int y = 0);

    // Métodos
    void disparar_grenade(int position_x, int position_y, char looking, GameMap* map, char id_player);

    void disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) override {
        disparar_grenade(position_x, position_y, looking, map, id_player);
    }

    void mostrarInformacion() const;  
    void update_weapon(int position_x, int position_y, char looking, GameMap* map, char id_player);

    // Getters
    std::string getNombre() const;
    double getAlcance() const;
    double getDispersion() const;
    int getMuniciones() const;

    virtual ~Grenade() = default;  // Destructor virtual
};

#endif  // GRENADE_H
