#include "move_down_command.h"
#include <iostream>


MoveDownCommand::MoveDownCommand(uint16_t player_id) : player_id(player_id) {}

void MoveDownCommand::execute() {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " se movió hacia abajo\n";
    
}
