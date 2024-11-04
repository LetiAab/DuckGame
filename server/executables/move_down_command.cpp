#include "move_down_command.h"
#include <iostream>
#include "common/constants.h"

MoveDownCommand::MoveDownCommand(uint16_t player_id) : player_id(player_id) {}

void MoveDownCommand::execute(Game& game) {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " se movió hacia abajo\n";
    
    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);
    duck->speed_y ++;

}
