#include "leave_gun_command.h"
#include <iostream>
#include "common/constants.h"
#include <typeinfo>
#include "../../guns/gun.h"
#include <memory>  // Para std::unique_ptr

LeaveGunCommand::LeaveGunCommand(uint16_t player_id) : player_id(player_id) {}

/*
void LeaveGunCommand::execute(Game& game) {

    
    std::cout << "Jugador " << player_id << " dispara\n";

    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);

    Arma* gun_to_delete = duck->gun;
    duck->gun = NULL;

    if (!gun_to_delete) {
        return;
    }

    // Verificar el tipo de arma y crear el proyectil correspondiente
    if (typeid(*gun_to_delete) == typeid(Granada)) {
        Granada* granada = static_cast<Granada*>(gun_to_delete);
        auto projectile = std::make_unique<ProyectilGranada>(granada->tiempo_explosion, duck->get_x() + 1, duck->get_y() + 1, duck->speed_x + 1, duck->speed_y);
        game.add_projectile(std::move(projectile));
    }

    if (typeid(*gun_to_delete) == typeid(Banana)) {
        Banana* banana = static_cast<Banana*>(gun_to_delete);
        auto projectile = std::make_unique<ProyectilBanana>(duck->get_x() + 1, duck->get_y() + 1, duck->speed_x + 1, duck->speed_y);
        game.add_projectile(std::move(projectile));
    }

    gun_to_delete->~Arma();
    delete gun_to_delete; 
}

*/
