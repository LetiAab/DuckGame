#ifndef SHOTGUN_H
#define SHOTGUN_H

#include <string>
#include <vector>
#include "bullet.h"
#include "../game/game_map.h"
#include "../game/item.h"  // Incluir Item
#include "common/constants.h"

class Bullet;  // Declaración anticipada de la clase Bullet
class GameMap; // Declaración anticipada de la clase GameMap

class Shotgun : public Item {  // Herencia de Item
public:
    std::vector<Bullet> bullets;

    // Constructor que inicializa la posición
    Shotgun(const std::string& nombre, double alcance, double dispersion, int municiones, int x = 0, int y = 0);

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

private:
    std::string nombre;
    double alcance;
    double dispersion;
    int municiones;
    bool recargando;
};

#endif  // SHOTGUN_H
