#include "gun.h"

#include "../game/game.h"


    Granada::Granada(Duck& owner) : Arma("Granada", 1, 5, owner), seguro_removido(false), tiempo_explosion(4) {}

    Proyectil* Granada::sacarSeguro() {
        seguro_removido = true;
        std::cout << "Seguro de la " << nombre << " removido. Explota en " << tiempo_explosion << " segundos.\n";
        return NULL;
    }

    void Granada::simulate_iteration() {
        if (seguro_removido) {
                tiempo_explosion += TIME_SLEEP;
        }

        if (tiempo_explosion >= 4000) {
                std::cout << "Explosión!" << std::endl;
                // Notar que debería devolver algo que diga que explotó, y entonces 
                // el mapa recibe eso, genera los fragmentos y elimina al objeto
        }
    }

    Proyectil* Granada::disparar() {
        return sacarSeguro();
    }

    Banana::Banana(Duck& owner) : Arma("Banana", 1, 5, owner) {}

    PewPewLaser::PewPewLaser(Duck& owner) : Arma("Pew-Pew Laser", 12, 37, owner) {}

    Proyectil* PewPewLaser::disparar() {
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

    LaserRifle::LaserRifle(Duck& owner) : Arma("Laser Rifle", 10, 30, owner) {}

    Proyectil* LaserRifle::disparar() {
        if (municiones > 0) {
            std::cout << "Disparando rayo láser que rebota con inclinación ajustable.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }

    AK47::AK47(Duck& owner) : Arma("AK-47", 30, 13, owner) {}

    Proyectil* AK47::disparar() {
        if (municiones > 0) {
            std::cout << "Disparando bala con dispersión y retroceso.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }

    PistolaDuelos::PistolaDuelos(Duck& owner) : Arma("Pistola de Duelos", 1, 5, owner) {}

    Proyectil* PistolaDuelos::disparar() {
        if (municiones > 0) {
            std::cout << "Disparando con dispersión considerable.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }

    PistolaCowboy::PistolaCowboy(Duck& owner) : Arma("Pistola Cowboy", 6, 20, owner) {}

    Proyectil* PistolaCowboy::disparar() {
        if (municiones > 0) {
            std::cout << "Disparando sin dispersión ni retroceso.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }

    Magnum::Magnum(Duck& owner) : Arma("Magnum", 6, 20, owner) {}

    Proyectil* Magnum::disparar() {
        if (municiones > 0) {
            std::cout << "Disparando con un poco de dispersión y retroceso.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }

    Escopeta::Escopeta(Duck& owner) : Arma("Escopeta", 2, 8, owner) {}

    Proyectil* Escopeta::disparar() {
        if (municiones > 0) {
            std::cout << "Disparando con gran dispersión. Recarga necesaria tras cada disparo.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }

    Sniper::Sniper(Duck& owner) : Arma("Sniper", 3, 64, owner) {}

    Proyectil* Sniper::disparar() {
        if (municiones > 0) {
            std::cout << "Disparando con precisión sin dispersión ni retroceso.\n";
            municiones--;
        } else {
            std::cout << "Sin municiones. Recarga necesaria.\n";
        }
    }