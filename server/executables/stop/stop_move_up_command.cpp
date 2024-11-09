#include "stop_move_up_command.h"
#include <iostream>
#include "common/constants.h"


StopMoveUpCommand::StopMoveUpCommand(uint16_t player_id) : player_id(player_id) {}

void StopMoveUpCommand::execute(Game& game) {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " dejo de saltar\n";
    
    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);
    duck->speed_y = 0;
    duck->is_jumping = false;
}