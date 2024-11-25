#ifndef BANANA_H
#define BANANA_H

#include <string>
#include <vector>

#include "grenade_fragment.h"
#include "common/constants.h"
#include "../guns/weapon.h"

class GameMap; // Declaración anticipada de la clase GameMap

class Banana : public Weapon {  // Herencia de Item
public:
    bool peeled;
    Position speed;
    bool impacto;

    // Constructor que inicializa la posición
    explicit Banana(int x = 0, int y = 0);

    // Métodos
    void disparar_banana(int position_x, int position_y, char looking, GameMap* map, char id_player);

    void disparar(int position_x, int position_y, char looking, GameMap* map, char id_player) override {
        disparar_banana(position_x, position_y, looking, map, id_player);
    }

    void mostrarInformacion() const;  
//    bool update_weapon(int position_x, int position_y, char looking, GameMap* map, char id_player);

    // Getters
    std::string getNombre() const;
    double getAlcance() const;
    double getDispersion() const;
    int getMuniciones() const;

    void update_weapon(GameMap& map);

    virtual ~Banana() = default;  // Destructor virtual
};

#endif  // BANANA_H
