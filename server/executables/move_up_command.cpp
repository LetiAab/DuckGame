#include "move_up_command.h"
#include <iostream>


MoveUpCommand::MoveUpCommand(uint16_t player_id) : player_id(player_id) {}

void MoveUpCommand::execute(Game& game) {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " se movió hacia arriba\n";
    
    Duck* duck = game.getDuckById(player_id);    
    duck->update_position(duck->get_x(), duck->get_y() - 1);
}