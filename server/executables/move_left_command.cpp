#include "move_left_command.h"
#include <iostream>


MoveLeftCommand::MoveLeftCommand(uint16_t player_id) : player_id(player_id) {}

void MoveLeftCommand::execute(Game& game) {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " se movió a la izquierda\n";
    
    Duck* duck = game.getDuckById(player_id);    
    duck->update_position(duck->get_x() - 1, duck->get_y());
}
