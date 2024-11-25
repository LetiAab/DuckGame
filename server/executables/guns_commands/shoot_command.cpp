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

        //si es un arma disparo, simplemente disparo
        if(msg.item_id != ARMOR_ID && msg.item_id != HELMET_ID){

            if(duck->disparar()){
                //hago como un handshaking de disparar para saber si tengo que  
                //reproducir el sonido de la bala
                Message msg2;
                msg2.type = SHOOT; 
                msg2.player_id = player_id;
                game.game_broadcast(msg2);
                std::cout << "Jugador " << player_id << " dispara con su arma le aviso al client\n";

            }
        }
    } else {
        //hago como un handshaking de disparar para saber si tengo que  
        //reproducir el sonido de la bala

        if(duck->disparar()){
            Message msg3;
            msg3.type = SHOOT; 
            msg3.player_id = player_id;
            game.game_broadcast(msg3);
            std::cout << "Jugador " << player_id << " dispara con su arma le aviso al client\n";
        } 
    }
}
