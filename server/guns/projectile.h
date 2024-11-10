#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <iostream>
#include <string>

#define TIME_SLEEP 20

class Game;

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

    ProyectilGranada(int tiempo, int pos_x, int pos_y, int speed_x, int speed_y);

    void simular(Game& game) override;

    void impactar() override ;
};

class ProyectilLaser : public Proyectil {
public:
    ProyectilLaser(int pos_x, int pos_y, int speed_x, int speed_y);
    
    void simular(Game& game) override;

    void impactar() override;
};

class ProyectilBanana : public Proyectil {
public:
    ProyectilBanana(int pos_x, int pos_y, int speed_x, int speed_y);

    void simular(Game& game) override;

    void impactar() override;
};

#endif // PROJECTILE_H
