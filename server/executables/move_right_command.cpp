#include "move_right_command.h"
#include <iostream>


MoveRightCommand::MoveRightCommand(uint16_t player_id) : player_id(player_id) {}

void MoveRightCommand::execute(Game& game) {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " se movió a la derecha\n";
                
    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);
    std::cout << "Despues de getDuck \n";
    std::cout << "Duck es" << duck << "\n";
    duck->speed_x ++;
    std::cout << "Despues de incr speed x \n";
}
