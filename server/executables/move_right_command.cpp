#include "move_right_command.h"
#include <iostream>


MoveRightCommand::MoveRightCommand(uint16_t player_id) : player_id(player_id) {}

void MoveRightCommand::execute(Game& game) {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " se movió a la derecha\n";

    Duck* duck = game.getDuckById(player_id);    
    duck->update_position(duck->get_x() + 1, duck->get_y());
}
