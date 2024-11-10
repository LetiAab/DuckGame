#include <iostream>
#include <string>
#include <vector>
#include <cmath>


#include "../guns/projectile.h"

#include "../game/duck.h"

#define TIME_SLEEP 20

class Arma {
protected:
    std::string nombre;
    int municiones;
    int alcance;
    Duck& owner;

public:
    Arma(const std::string& nombre, int municiones, int alcance, Duck& owner)
        : nombre(nombre), municiones(municiones), alcance(alcance), owner(owner) {}

    virtual Proyectil* disparar() = 0;
    virtual void recargar() {
        std::cout << nombre << " recargado.\n";
    }

    virtual ~Arma() = default;
};

class Granada : public Arma {
    bool seguro_removido;
public:
    int tiempo_explosion;

    Granada(Duck& owner);

    Proyectil* sacarSeguro();

    void simulate_iteration();

    Proyectil* disparar() override;
};

class Banana : public Arma {
public:
    Banana(Duck& owner);
};

class PewPewLaser : public Arma {
public:
    PewPewLaser(Duck& owner);

    Proyectil* disparar() override;
};

class LaserRifle : public Arma {
public:
    LaserRifle(Duck& owner);

    Proyectil* disparar() override;
};

class AK47 : public Arma {
public:
    AK47(Duck& owner);

    Proyectil* disparar() override;
};

class PistolaDuelos : public Arma {
public:
    PistolaDuelos(Duck& owner);

    Proyectil* disparar() override;
};

class PistolaCowboy : public Arma {
public:
    PistolaCowboy(Duck& owner);

    Proyectil* disparar() override;
};

class Magnum : public Arma {
public:
    Magnum(Duck& owner);

    Proyectil* disparar() override;
};

class Escopeta : public Arma {
public:
    Escopeta(Duck& owner);

    Proyectil* disparar() override;
};

class Sniper : public Arma {
public:
    Sniper(Duck& owner);

    Proyectil* disparar() override;
};
