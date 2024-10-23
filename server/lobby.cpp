#include "lobby.h"
#include "match.h"

#include "common/lobby_message.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>


Lobby::Lobby(): is_alive(true), players(), matches(), lobby_queue(){}

void Lobby::run() {

    while(is_alive){

        LobbyCommand cmd = lobby_queue.pop();
        process_command(cmd);
    }

}

void Lobby::add_player(Player* player){

    send_first_message(player);
    players.push_back(player);

}

void Lobby::stop() {
    is_alive = false;

}

Queue<LobbyCommand>& Lobby::get_lobby_queue(){
    return lobby_queue;
}

void Lobby::send_first_message(Player* player){
    LobbyMessage msg;
    msg.player_id = player->get_player_id();
    //llenar con la info de los match, etc

    player->send_lobby_message(msg);

}

void Lobby::process_command(const LobbyCommand& cmd){
    uint8_t type = cmd.type;

    if (type == 0){
        std::cout << "hola";
    }
    /*
    if (cmd.type == COMENZAR_PARTIDA) {
        //comenzar partida y marcarla como no disponible 

    } else if(cmd.type == NUEVA_PARTIDA) {
        //crear un match nuevo, agregarlo a la lista, y sumar al jugador
    
    } else if(cmd.type == PARTIDA_EXISTENTE){
        //si la partida existe, no empezo y no supero el limite de jugadores
        //agregar al player a dicha partida

        //avisar al jugador si no pudo conectarse a esa partida

    } else {
        //salir: sacar al jugador
    }
   
    */
}