#include "shoot_command.h"
#include <iostream>
#include "common/constants.h"

#include "../guns/gun.h"
#include "../guns/projectile.h"

ShootCommand::ShootCommand(uint16_t player_id) : player_id(player_id) {}

void ShootCommand::execute(Game& game) {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " dispara\n";

    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);

    // Ejecutar el disparo: veo si hay balas, resto las balas, pongo la bala en el mapa y le doy un movimiento
    Arma* gun = duck->gun;
    if (!gun)
        return;
    Proyectil* projectile = gun->disparar();
}
