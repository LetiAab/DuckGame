#include "lobby_player.h"

#include <iostream>
#include <utility>
#include "common/liberror.h"



LobbyPlayer::LobbyPlayer(Socket&& socket, const uint16_t id, Queue<LobbyCommand>& lobby_queue):
        protocol(std::move(socket)),
        player_id(id),
        lobby_receiver(protocol, lobby_queue),
        connected(true),
        playing(false),
        match_id(0) {}

bool LobbyPlayer::send_lobby_message(const LobbyMessage& msg) {
    try{
        protocol.send_lobby_message(msg);
        return true;

    } catch(const LibError& e){
        return false;
    }
}

void LobbyPlayer::start() {
    lobby_receiver.start();
}


std::shared_ptr<Player> LobbyPlayer::start_game(Queue<std::shared_ptr<Executable>>& game_queue) {
    //detengo el hilo para que el cliente no hable mas con el lobby
    playing = true;
    lobby_receiver.stop();
    lobby_receiver.join();

    std::shared_ptr<Player> player = std::make_shared<Player>(std::move(protocol), player_id, game_queue);
    return  player;

}


void LobbyPlayer::stop() {
    if(!playing){
        protocol.shutdown();
    }
    lobby_receiver.stop();
    lobby_receiver.join();
}

uint16_t LobbyPlayer::get_player_id() { return player_id; }

void LobbyPlayer::set_match_id(uint16_t id){
    match_id = id;
}

uint16_t LobbyPlayer::get_match_id() { return match_id; }

bool LobbyPlayer::is_playing() { return playing;}

bool LobbyPlayer::is_connected() { return (lobby_receiver.is_running()); }

