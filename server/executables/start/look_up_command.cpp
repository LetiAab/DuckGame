#include "look_up_command.h"
#include <iostream>
#include "../../../common/constants.h"

LookUpCommand::LookUpCommand(uint16_t player_id) : player_id(player_id) {}

void LookUpCommand::execute(Game& game) {
    // Aquí debes implementar la lógica del comando para mirar hacia arriba.
    std::cout << "Jugador " << player_id << " mira hacia arriba.\n";

    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);
    duck->is_looking_up = true;

    //Aviso que estoy mirando para arriba
    Message lookUpMsg;
    duck->get_duck_position_message(lookUpMsg);
    game.game_broadcast(lookUpMsg);


}
