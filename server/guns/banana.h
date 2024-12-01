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
    bool pisada;

    // Constructor que inicializa la posición
    explicit Banana(int x = 0, int y = 0, Position speed = Position {0, 0});

    // Métodos
    bool disparar_banana(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up);

    bool disparar(int position_x, int position_y, char looking, GameMap* map, char id_player, bool is_looking_up) override {
        return disparar_banana(position_x, position_y, looking, map, id_player, is_looking_up);
    }

    void mostrarInformacion() const;  
//    bool update_weapon(int position_x, int position_y, char looking, GameMap* map, char id_player);

    void update_weapon(GameMap& map);

    void prepare_drop(char duck_looking);

    virtual ~Banana() = default;  // Destructor virtual
};

#endif  // BANANA_H
