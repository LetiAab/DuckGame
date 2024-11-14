#include "take_item_command.h"
#include <iostream>

TakeItemCommand::TakeItemCommand(uint16_t player_id) : player_id(player_id) {}

void TakeItemCommand::execute(Game& game) {

    std::cout << "Jugador " << player_id << " quiere agarrar item\n";

    char char_id = static_cast<char>(player_id + '0');
    Duck* duck = game.getDuckById(char_id);
    Item* item = game.getItemByPosition(duck->getPosition());

    duck->pickUpItem(item);

}
