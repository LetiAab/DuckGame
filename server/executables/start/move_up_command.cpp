#include "move_up_command.h"
#include <iostream>

#include "common/constants.h"


MoveUpCommand::MoveUpCommand(uint16_t player_id) : player_id(player_id) {}

void MoveUpCommand::execute(Game& game) {
    // esto va a recibir el juego y una lista de mensajes tal vez
    std::cout << "Jugador " << player_id << " se movió hacia arriba\n";
    

    char char_id = static_cast<char>(player_id + '0');

    Duck* duck = game.getDuckById(char_id);

    if(duck->is_jumping){
        //si estaba saltando antes, significa que empieza a aletear
        //duck->speed_y -= 1;
        duck->is_fluttering = true;
    } else {
        //si empieza a saltar
        duck->speed_y -= DUCK_JUMP_POWER; // Si queremos que salte N casillas cambiar por N
        duck->is_jumping = true;
    }


}
