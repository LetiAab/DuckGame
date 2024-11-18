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

        std::cout << "El item de la mano es de ID " << duck->getItemOnHand()->getItemId() << "\n";

        Message msg;
        msg.type = DUCK_EQUIP_ITEM;
        msg.item_id = duck->getItemOnHand()->getItemId();
        msg.player_id = player_id;

        //aviso que el pato se equipo un item
        game.game_broadcast(msg);

        std::cout << "Antes del useOnHand \n";
        
        duck->useOnHand();  //se equipa el item
        std::cout << "Saliendo del agarrar item \n";

        //si es un arma disparo
        if(msg.item_id != ARMOR_ID && msg.item_id != HELMET_ID){
            duck->disparar();
        }
    } else {
        std::cout << "Jugador " << player_id << " dispara con su arma\n";
        duck->disparar(); 
    }
}
