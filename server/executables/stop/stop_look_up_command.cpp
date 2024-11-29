#include "stop_look_up_command.h"
#include <iostream>
#include "common/constants.h"

StopLookUpCommand::StopLookUpCommand(uint16_t player_id) : player_id(player_id) {}

void StopLookUpCommand::execute(Game& game) {
    // Aquí debes implementar la lógica para detener el gesto de mirar hacia arriba.
    std::cout << "Jugador " << player_id << " dejó de mirar hacia arriba.\n";

    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);
    duck->is_looking_up = false;
}
