#include <iostream>
#include <string>

#include "../game/game.h"

#define TIME_SLEEP 20

class Proyectil {
protected:
    std::string tipo;
    int alcance;
    int daño;

public:
    int speed_x;
    int speed_y;

    Proyectil(const std::string& tipo, int alcance, int daño, int speed_x, int speed_y)
        : tipo(tipo), alcance(alcance), daño(daño), speed_x(speed_x), speed_y(speed_y)  {}

    virtual void simular() = 0;  // Define el movimiento en cada tipo de proyectil
    virtual void impactar() = 0;  // Define el comportamiento al impactar

    virtual ~Proyectil() = default;
};

class ProyectilGranada : public Proyectil {
public:
    int tiempo_explosion;

    ProyectilGranada(int tiempo, int speed_x, int speed_y) : Proyectil("Granada", 5, 15, speed_x, speed_y) {
        tiempo_explosion = tiempo;
    }

    void simular() override {
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
    ProyectilLaser(int speed_x, int speed_y) : Proyectil("Rayo Láser", 38, 15, speed_x, speed_y) {}

    void simular() override {
        std::cout << "El rayo láser avanza en línea recta con alcance de " << alcance << " tiles.\n";
    }

    void impactar() override {
        std::cout << "El rayo láser impacta causando " << daño << " de daño.\n";
    }
};

class ProyectilBanana : public Proyectil {
public:
    ProyectilBanana(int speed_x, int speed_y) : Proyectil("Banana", 0, 0, speed_x, speed_y) {}

    void simular() override {
        std::cout << "La banana se queda en el suelo.\n";
    }

    void impactar() override {
        std::cout << "Un pato pisa la banana y se resbala.\n";
    }
};
