#include "take_item_command.h"
#include <iostream>
#include "common/constants.h"

TakeItemCommand::TakeItemCommand(uint16_t player_id) : player_id(player_id) {}

void TakeItemCommand::execute(Game& game) {

    std::cout << "Jugador " << player_id << " quiere agarrar item\n";

    //Refactor! No creo que haga falta buscar el spawn place y el item
    char char_id = static_cast<char>(player_id + '0');
    Duck* duck = game.getDuckById(char_id);
    std::shared_ptr<Item> item = game.getItemByPosition(duck->getPosition());
    SpawnPlace* spawn = game.getSpawnPlaceByPosition(duck->getPosition());

    //No se bien donde mandar el mensaje de PATO_ID agarro ITEM_ID (lo puse aca)
    //si agarre algo aviso!
    if(duck->pickUpItem(item)){

        //le aviso al spawn place que ahora tiene el item con id 0 (ninguno)
        Message msg1;
        spawn->setItemId(0);
        spawn->getSpawnPlaceItemUpdateMessage(msg1);
        game.game_broadcast(msg1);

        //le aviso al jugador que agarro un item y ahora lo tiene que tener en la mano
        Message msg2; 
        msg2.type = DUCK_PICKUP_ITEM;
        msg2.player_id = player_id;
        msg2.item_id = item->getItemId();
        game.game_broadcast(msg2);
    
    }

}
