#ifndef LASER_RIFLE_H
#define LASER_RIFLE_H

#include <string>
#include <vector>
#include "bouncing_laser.h"
#include "../game/game_map.h"
#include "../game/item.h"  // Incluir Item
#include "common/constants.h"

class BouncingLaser;  // Declaración anticipada de la clase Bullet
class GameMap; // Declaración anticipada de la clase GameMap 

class LaserRifle : public Item {  // Herencia de Item
public:
    std::vector<BouncingLaser> lasers;

    // Constructor que inicializa la posición
    LaserRifle(const std::string& nombre, double alcance, double dispersion, int municiones, int x = 0, int y = 0);

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
};

#endif  // LASER_RIFLE_H

/* #ifndef LASER_RIFLE_H
#define LASER_RIFLE_H


#include "bouncing_laser.h"

class LaserRifle : public Weapon {
public:
    LaserRifle(int x = 0, int y = 0);  // Constructor con posición opcional

    std::vector<BouncingLaser> lasers;  // Reemplazar bullets por lasers

    void disparar(int position_x, int position_y, char looking, GameMap* map, char id_player);
    void update_weapon();

private:
    double current_angle;  // Ángulo actual de disparo (en grados)
    int rebound_limit;     // Número máximo de rebotes permitidos
    std::string nombre;
    double alcance;
    double dispersion;
    int municiones;
};

#endif  // LASER_RIFLE_H
 */