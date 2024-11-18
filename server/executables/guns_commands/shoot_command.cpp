#include "shoot_command.h"
#include <iostream>
#include "common/constants.h"

ShootCommand::ShootCommand(uint16_t player_id) : player_id(player_id) {}

void ShootCommand::execute(Game& game) {
    std::cout << "Jugador " << player_id << " dispara\n";

    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);

    if (duck->onHand) {
        std::cout << "Jugador " << player_id << " se equipa el item en mano\n";
        Message msg;
        msg.type = DUCK_EQUIP_ITEM;
        msg.item_id = duck->getItemOnHand()->getItemId();
        msg.player_id = player_id;

        //aviso que el pato se equipo un item
        game.game_broadcast(msg);

        
        duck->useOnHand();  //se equipa el item
    } else {
        std::cout << "Jugador " << player_id << " dispara con su arma\n";
        duck->disparar(); 
    }
}
