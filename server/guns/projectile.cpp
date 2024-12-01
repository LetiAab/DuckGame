#include "projectile.h"

#include "../game/game.h"

Projectile::Projectile(int projectile_id, int projectile_type, Position position, int direction_x, int direction_y, GameMap* map, char duck_id, int alcance, bool horizontal) 
    : projectile_id(projectile_id),
    projectile_type(projectile_type),
    position(position),
    old_position(position),
    speed(direction_x, direction_y),
    direction_x(direction_x),
    direction_y(direction_y),
    map(map),
    impacto(false),
    duck_id(duck_id),
    alcance(alcance),
    should_erase(false),
    horizontal(horizontal) {}

/* 
    ProyectilGranada::ProyectilGranada(int tiempo, int pos_x, int pos_y, int speed_x, int speed_y) : Proyectil("Granada", 5, 15, pos_x, pos_y, speed_x, speed_y) {
        tiempo_explosion = tiempo;
    }

    void ProyectilGranada::simular(Game& game) {
        // Mover el proyectil
        game.map.move_projectile(pos_x, pos_y, speed_x, speed_y, 1, 1);

        tiempo_explosion += TIME_SLEEP;

        if (tiempo_explosion >= 4000) {
                std::cout << "Explosión!" << std::endl;
                // Notar que debería devolver algo que diga que explotó, y entonces 
                // el mapa recibe eso, genera los fragmentos y elimina al objeto
        }
    }

    void ProyectilGranada::impactar() {
        std::cout << "El rayo láser impacta causando " << daño << " de daño.\n";
    }


ProyectilLaser::ProyectilLaser(int pos_x, int pos_y, int speed_x, int speed_y) : Proyectil("Rayo Láser", 38, 15, pos_x, pos_y, speed_x, speed_y) {}

void ProyectilLaser::simular(Game& game) {
        // Mover el proyectil
        game.map.move_projectile(pos_x, pos_y, speed_x, speed_y, 1, 1);

        std::cout << "El rayo láser avanza en línea recta con alcance de " << alcance << " tiles.\n";
    }

void ProyectilLaser::impactar() {
        std::cout << "El rayo láser impacta causando " << daño << " de daño.\n";
    }

    ProyectilBanana::ProyectilBanana(int pos_x, int pos_y, int speed_x, int speed_y) : Proyectil("Banana", 0, 0, pos_x, pos_y, speed_x, speed_y) {}

    void ProyectilBanana::simular(Game& game) {
        // Mover hasta que toque el piso, entonces queda esperando que alguno la pise
        game.map.move_projectile(pos_x, pos_y, speed_x, speed_y, 1, 1);

        
        // Chequear si tocó el piso. Si es así cambiar las velocidades por 0
        if (game.map.is_element_touching_floor(pos_x, pos_y, 1, 1)) {
                speed_x = 0;
                speed_y = 0;
        }
        

        // Si hay un pato que está ocupando la misma posición que yo entonces le activo un flag patinando o simplemente le hago daño
        
        char duck_id = game.map.duck_in_position(pos_x, pos_y, 1, 1);
        Duck* duck = game.getDuckById(duck_id);
        if (duck) {
                // Es una o la otra, lo del slippy no me gusta como idea, ver si va a ser necesario
                duck->is_slippy = true;
                duck->life_points -= 10;
        }
       

        std::cout << "La banana se queda en el suelo.\n";
    }

    void ProyectilBanana::impactar() {
        std::cout << "Un pato pisa la banana y se resbala.\n";
    }
 */
