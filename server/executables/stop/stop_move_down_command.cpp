#include "stop_move_down_command.h"

StopMoveDownCommand::StopMoveDownCommand(uint16_t player_id) : player_id(player_id) {}

void StopMoveDownCommand::execute(Game& game) {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " dejo de moverse hacia abajo\n";

    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);
    duck->is_moving = false;
    duck->is_laying_down = false;
    duck->speed_x = 0;
    duck->speed_y = 0;
}