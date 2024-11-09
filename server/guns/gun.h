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

public:
    Arma(const std::string& nombre, int municiones, int alcance)
        : nombre(nombre), municiones(municiones), alcance(alcance) {}

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

    Granada() : Arma("Granada", 1, 5), seguro_removido(false), tiempo_explosion(4) {}

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
    Banana() : Arma("Banana", 1, 5) {}

    Proyectil* lanzar() {
        std::cout << "Lanzando " << nombre << " en el suelo. ¡Cuidado con el resbalón!\n";

    }

    Proyectil* disparar() override {
        lanzar();
    }
};

class PewPewLaser : public Arma {
public:
    PewPewLaser() : Arma("Pew-Pew Laser", 12, 37) {}

    Proyectil* disparar() override {
        if (municiones > 0) {
            std::cout << "Disparando ráfaga de 3 rayos con algo de dispersión.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }
};

class LaserRifle : public Arma {
public:
    LaserRifle() : Arma("Laser Rifle", 10, 30) {}

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
    AK47() : Arma("AK-47", 30, 13) {}

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
    PistolaDuelos() : Arma("Pistola de Duelos", 1, 5) {}

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
    PistolaCowboy() : Arma("Pistola Cowboy", 6, 20) {}

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
    Magnum() : Arma("Magnum", 6, 20) {}

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
    Escopeta() : Arma("Escopeta", 2, 8) {}

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
    Sniper() : Arma("Sniper", 3, 64) {}

    Proyectil* disparar() override {
        if (municiones > 0) {
            std::cout << "Disparando con precisión sin dispersión ni retroceso.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }
};
