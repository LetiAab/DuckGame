#include "shoot_command.h"
#include <iostream>
#include "common/constants.h"

#include "../../guns/gun.h"
#include "../../guns/projectile.h"

ShootCommand::ShootCommand(uint16_t player_id) : player_id(player_id) {}

void ShootCommand::execute(Game& game) {
    std::cout << "Jugador " << player_id << " dispara\n";

    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);

    if (duck->onHand) {
        std::cout << "Jugador " << player_id << " usa el item en mano\n";
        duck->useOnHand(); 
    } else {
        std::cout << "Jugador " << player_id << " dispara con su arma\n";
        duck->disparar(); 
    }



}
