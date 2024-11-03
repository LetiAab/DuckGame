#include "move_left_command.h"
#include <iostream>


MoveLeftCommand::MoveLeftCommand(uint16_t player_id) : player_id(player_id) {}

void MoveLeftCommand::execute(Game& game) {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " se movió a la izquierda\n";
    
    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);
    duck->speed_x --;
}
