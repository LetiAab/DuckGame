#include "stop_move_right_command.h"
#include <iostream>
#include "common/constants.h"


StopMoveRightCommand::StopMoveRightCommand(uint16_t player_id) : player_id(player_id) {}

void StopMoveRightCommand::execute(Game& game) {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " dejo de moverse\n";
    
    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);
    if (duck->speed_x > 0) {
        duck->speed_x = 0;
    }
    duck->is_moving = false;
}
