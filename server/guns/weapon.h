#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <vector>
#include "bullet.h"
#include "../game/game_map.h"


class Bullet;  // Declaración anticipada de la clase Bullet
class GameMap; // Declaración anticipada de la clase GameMap

class Weapon {
public:

    std::vector<Bullet> bullets;

    // Constructor
    Weapon(const std::string& nombre, double alcance, double dispersion, int municiones);

    // Métodos
    void disparar(int position_x, int position_y, char looking, GameMap* map, char id_player);
    void recargar(int cantidad);  
    void mostrarInformacion() const;  

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
};

#endif  // WEAPON_H