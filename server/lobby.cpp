#include "lobby.h"
#include "match.h"

#include "common/lobby_message.h"
#include "common/constants.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>


Lobby::Lobby(): is_alive(true), players(), matches(), lobby_queue(), match_counter_ids(0){}

void Lobby::run() {
    try {
        while(is_alive){

            LobbyCommand cmd = lobby_queue.pop();
            process_command(cmd);
        }

    } catch (const ClosedQueue& e){
        std::cout << "Se cerro la queue del lobby\n";

    }

}

void Lobby::add_player(Player* player){

    send_first_message(player);
    players.push_back(player);

}

void Lobby::stop() {
    is_alive = false;
    lobby_queue.close();
    //liberar memoria, etc

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

Player* Lobby::find_player_by_id(const int id) {
    for (Player* player : players) {
        if (player->get_player_id() == id) {
            return player; 
        }
    }
    return nullptr;
}

Match* Lobby::find_match_by_id(const int id) {
    for (Match* match : matches) {
        if (match->get_match_id() == id) {
            return match; 
        }
    }
    return nullptr;
}

void Lobby::process_command(const LobbyCommand& cmd){

    Player* player = find_player_by_id(cmd.player_id);
    uint8_t type = cmd.type;

    
    if (type == NEW_MATCH) {
        Match* new_match = new Match(match_counter_ids);
        matches.push_back(new_match);

        new_match->add_player(player);
        std::cout << "El jugador creo una nueva partida\n";
        //mandar un mensaje al jugador con el id de su partida
        match_counter_ids += 1;


    } else if(type == EXISTING_MATCH) {
        Match* match = find_match_by_id(cmd.match_id);

        if(match->add_player(player)){
            std::cout << "El jugador se conecto a la partida existente\n";
            //mandar un mensaje al jugador con el id de su partida
        } else{
            std::cout << "El jugador NO pudo conectarse a la partida\n";
            //enviarle al jugador que no pudo conectarse
        }

    
    } else if(cmd.type == START_MATCH){
        Match* match = find_match_by_id(cmd.match_id);

        if(match->start_match()){
            std::cout << "La partida se inició exitosamente\n";
            //avisarle al jugador 

        } else{
            std::cout << "No se puede iniciar esta partida aun\n";
            //avisarle al jugador 
        }


    } else {
        //salir: sacar al jugador
    }
   

}