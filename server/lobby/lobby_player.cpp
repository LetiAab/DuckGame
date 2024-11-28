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
        std::cout << "Lobby Player: mande el mensaje del lobby"  << std::endl;
        return true;

    } catch(const LibError& e){
        connected = false;
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

    //en lugar de mover el protocolo podria crear uno nuevo exclusivo de Game
    std::shared_ptr<Player> player = std::make_shared<Player>(std::move(protocol), player_id, game_queue);
    return  player;

}


void LobbyPlayer::stop() {
    connected = false;
    lobby_receiver.stop();
    std::cout << "LobbyPlayer: estoy por hacer el join"  << std::endl;
    lobby_receiver.join();
    std::cout << "LobbyPlayer: hice el join"  << std::endl;
}

uint16_t LobbyPlayer::get_player_id() { return player_id; }

void LobbyPlayer::set_match_id(uint16_t id){
    match_id = id;
}

uint16_t LobbyPlayer::get_match_id() { return match_id; }

bool LobbyPlayer::is_playing() { return playing;}

bool LobbyPlayer::is_connected() { return (lobby_receiver.is_running()); }

