#include <iostream>
#include <string>

#include "../game/game.h"

#define TIME_SLEEP 20

class Proyectil {
protected:
    std::string tipo;
    int alcance;
    int daño;
    int pos_x;
    int pos_y;

public:
    int speed_x;
    int speed_y;

    Proyectil(const std::string& tipo, int alcance, int daño, int pos_x, int pos_y, int speed_x, int speed_y)
        : tipo(tipo), alcance(alcance), daño(daño), pos_x(pos_x), pos_y(pos_y), speed_x(speed_x), speed_y(speed_y)  {}

    virtual void simular(Game& game) = 0;  // Define el movimiento en cada tipo de proyectil
    virtual void impactar() = 0;  // Define el comportamiento al impactar

    virtual ~Proyectil() = default;
};

class ProyectilGranada : public Proyectil {
public:
    int tiempo_explosion;

    ProyectilGranada(int tiempo, int pos_x, int pos_y, int speed_x, int speed_y) : Proyectil("Granada", 5, 15, pos_x, pos_y, speed_x, speed_y) {
        tiempo_explosion = tiempo;
    }

    void simular(Game& game) override {
        // Falta mover el proyectil

        tiempo_explosion += TIME_SLEEP;

        if (tiempo_explosion >= 4000) {
                std::cout << "Explosión!" << std::endl;
                // Notar que debería devolver algo que diga que explotó, y entonces 
                // el mapa recibe eso, genera los fragmentos y elimina al objeto
        }
    }

    void impactar() override {
        std::cout << "El rayo láser impacta causando " << daño << " de daño.\n";
    }
};

class ProyectilLaser : public Proyectil {
public:
    ProyectilLaser(int pos_x, int pos_y, int speed_x, int speed_y) : Proyectil("Rayo Láser", 38, 15, pos_x, pos_y, speed_x, speed_y) {}

    void simular(Game& game) override {
        std::cout << "El rayo láser avanza en línea recta con alcance de " << alcance << " tiles.\n";
    }

    void impactar() override {
        std::cout << "El rayo láser impacta causando " << daño << " de daño.\n";
    }
};

class ProyectilBanana : public Proyectil {
public:
    ProyectilBanana(int pos_x, int pos_y, int speed_x, int speed_y) : Proyectil("Banana", 0, 0, pos_x, pos_y, speed_x, speed_y) {}

    void simular(Game& game) override {
        // Mover hasta que toque el piso, entonces queda esperando que alguno la pise
        
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

    void impactar() override {
        std::cout << "Un pato pisa la banana y se resbala.\n";
    }
};
