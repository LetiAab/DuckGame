#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "../game/game.h"

#include "../guns/projectile.h"

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

    Granada(Duck& owner) : Arma("Granada", 1, 5, owner), seguro_removido(false), tiempo_explosion(4) {}

    Proyectil* sacarSeguro() {
        seguro_removido = true;
        std::cout << "Seguro de la " << nombre << " removido. Explota en " << tiempo_explosion << " segundos.\n";
        return NULL;
    }

    void simulate_iteration() {
        if (seguro_removido) {
                tiempo_explosion += TIME_SLEEP;
        }

        if (tiempo_explosion >= 4000) {
                std::cout << "Explosión!" << std::endl;
                // Notar que debería devolver algo que diga que explotó, y entonces 
                // el mapa recibe eso, genera los fragmentos y elimina al objeto
        }
    }

    Proyectil* disparar() override {
        return sacarSeguro();
    }
};

class Banana : public Arma {
public:
    Banana(Duck& owner) : Arma("Banana", 1, 5, owner) {}
};

class PewPewLaser : public Arma {
public:
    PewPewLaser(Duck& owner) : Arma("Pew-Pew Laser", 12, 37, owner) {}

    Proyectil* disparar() override {
        if (municiones > 0) {
            std::cout << "Disparando ráfaga de 3 rayos con algo de dispersión.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }

        // Raro, no me sirve usar punteros salvo para los casos NULL, capaz puedo hacer un objeto ProyectilNull, y listo
        ProyectilLaser projectile(owner.get_x() + 1, owner.get_y() + 1, owner.speed_x + 1, owner.speed_y);
        return &projectile;
    }
};

class LaserRifle : public Arma {
public:
    LaserRifle(Duck& owner) : Arma("Laser Rifle", 10, 30, owner) {}

    Proyectil* disparar() override {
        if (municiones > 0) {
            std::cout << "Disparando rayo láser que rebota con inclinación ajustable.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }
};

class AK47 : public Arma {
public:
    AK47(Duck& owner) : Arma("AK-47", 30, 13, owner) {}

    Proyectil* disparar() override {
        if (municiones > 0) {
            std::cout << "Disparando bala con dispersión y retroceso.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }
};

class PistolaDuelos : public Arma {
public:
    PistolaDuelos(Duck& owner) : Arma("Pistola de Duelos", 1, 5, owner) {}

    Proyectil* disparar() override {
        if (municiones > 0) {
            std::cout << "Disparando con dispersión considerable.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }
};

class PistolaCowboy : public Arma {
public:
    PistolaCowboy(Duck& owner) : Arma("Pistola Cowboy", 6, 20, owner) {}

    Proyectil* disparar() override {
        if (municiones > 0) {
            std::cout << "Disparando sin dispersión ni retroceso.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }
};

class Magnum : public Arma {
public:
    Magnum(Duck& owner) : Arma("Magnum", 6, 20, owner) {}

    Proyectil* disparar() override {
        if (municiones > 0) {
            std::cout << "Disparando con un poco de dispersión y retroceso.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }
};

class Escopeta : public Arma {
public:
    Escopeta(Duck& owner) : Arma("Escopeta", 2, 8, owner) {}

    Proyectil* disparar() override {
        if (municiones > 0) {
            std::cout << "Disparando con gran dispersión. Recarga necesaria tras cada disparo.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }
};

class Sniper : public Arma {
public:
    Sniper(Duck& owner) : Arma("Sniper", 3, 64, owner) {}

    Proyectil* disparar() override {
        if (municiones > 0) {
            std::cout << "Disparando con precisión sin dispersión ni retroceso.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }
};
