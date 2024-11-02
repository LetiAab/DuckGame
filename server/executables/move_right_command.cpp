#include "move_right_command.h"
#include <iostream>


MoveRightCommand::MoveRightCommand(uint16_t player_id) : player_id(player_id) {}

void MoveRightCommand::execute() {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " se movió a la derecha\n";
    
}
