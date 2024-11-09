#include "leave_gun_command.h"
#include <iostream>
#include "common/constants.h"
#include <typeinfo>

#include "../guns/gun.h"

LeaveGunCommand::LeaveGunCommand(uint16_t player_id) : player_id(player_id) {}

void LeaveGunCommand::execute(Game& game) {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " dispara\n";

    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);

    // Drop the gun and delete it
    Arma* gun_to_delete = duck->gun;
    duck->gun = NULL;
    if (!gun_to_delete) {
        return;
    }
    
    if (typeid(gun_to_delete) == typeid(Granada)) {
        // Dudo que funcione por temas de ownership
        Granada* granada = (Granada*)gun_to_delete;
        ProyectilGranada projectile(granada->tiempo_explosion, duck->speed_x + 1, duck->speed_y);
        game.add_projectile(std::move(projectile), duck->get_x() + 1, duck->get_y() + 1);
    }

    gun_to_delete->~Arma();
    delete(gun_to_delete);
}
