#include "take_item_command.h"
#include <iostream>
#include "../../../common/constants.h"

TakeItemCommand::TakeItemCommand(uint16_t player_id) : player_id(player_id) {}

void TakeItemCommand::execute(Game& game) {

    std::cout << "Jugador " << player_id << " quiere agarrar item\n";

    //Refactor! Estamos buscando demasiadas cosas cuando queremos hacer un pickup
    char char_id = static_cast<char>(player_id + '0');
    Duck* duck = game.getDuckById(char_id);
    std::shared_ptr<Item> item = game.getItemByPosition(duck->getPosition());
    std::shared_ptr<Item> item2 = game.getItemOnFloorByPosition(duck->getPosition());
    SpawnPlace* spawn = game.getSpawnPlaceByPosition(duck->getPosition());
    Box* box = game.getBoxByPosition(duck->getPosition());


    //No se bien donde mandar el mensaje de PATO_ID agarro ITEM_ID (lo puse aca)
    //si agarre algo DE UN SPAWN PLACE aviso!
    if(duck->pickUpItem(item)){

        //le aviso al spawn place que ahora tiene el item con id 0 (ninguno)
        if(spawn != nullptr){
            //pongo la condicion porque puede haber item y no haber spawn
            Message msg1;
            spawn->setItemId(0);
            spawn->getSpawnPlaceItemUpdateMessage(msg1);
            game.game_broadcast(msg1);
        }

        if(box != nullptr){
            Message msg2;
            box->setItemId(0);
            box->getBoxMessage(msg2);
            game.game_broadcast(msg2);
        }

        //le aviso al jugador que agarro un item y ahora lo tiene que tener en la mano
        Message msg3; 
        msg3.type = DUCK_PICKUP_ITEM;
        msg3.player_id = player_id;
        msg3.item_id = item->getItemId();
        game.game_broadcast(msg3);
    
    }

    //si agarre ALGO DEL PISO

    if(duck->pickUpItem(item2)){

        std::cout << "PICKUP ITEM 2 DEL PISO" << "\n";

        //le aviso al item que tiene que dejar de renderizarse   
        Message msg1;
        item2->getItemUpdate(msg1);
        game.game_broadcast(msg1);
        
        std::cout << "mando al cliente que agarre el item" << "\n";


        //le aviso al jugador que agarro un item y ahora lo tiene que tener en la mano
        Message msg3; 
        msg3.type = DUCK_PICKUP_ITEM;
        msg3.player_id = player_id;
        msg3.item_id = item2->getItemId();
        game.game_broadcast(msg3);

        std::cout << "mando al cliente que se ponga el item en la mano" << "\n";

    
    }

}
