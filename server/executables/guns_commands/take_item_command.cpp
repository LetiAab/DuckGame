#include "take_item_command.h"
#include <iostream>
#include "common/constants.h"

TakeItemCommand::TakeItemCommand(uint16_t player_id) : player_id(player_id) {}

void TakeItemCommand::execute(Game& game) {

    std::cout << "Jugador " << player_id << " quiere agarrar item\n";

    char char_id = static_cast<char>(player_id + '0');
    Duck* duck = game.getDuckById(char_id);
    Item* item = game.getItemByPosition(duck->getPosition());

    //No se bien donde mandar el mensaje de PATO_ID agarro ITEM_ID (lo puse aca)
    //si agarre algo aviso!
    if(duck->pickUpItem(item)){
        Message msg; 
        msg.type = DUCK_PICKUP_ITEM;
        msg.player_id = player_id;
        msg.item_id = item->getItemId();
        game.game_broadcast(msg);
    }


}
