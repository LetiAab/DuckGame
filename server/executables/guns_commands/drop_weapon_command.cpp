#include "drop_weapon_command.h"

DropWeaponCommand::DropWeaponCommand(uint16_t player_id)
    : player_id(player_id) {}

void DropWeaponCommand::execute(Game& game) {
    std::cout << "Jugador " << player_id << " deja caer su arma\n";

    char char_id = static_cast<char>(player_id + '0');
    Duck* duck = game.getDuckById(char_id);


    //si efectivamente solte un arma aviso que la solté. Sino no pasa nada
    if(duck->dropWeapon()){

        Message msg;
        msg.type = DROP_WEAPON;
        msg.player_id = player_id;

        game.game_broadcast(msg);
    }
    

}
