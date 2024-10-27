#include "lobby_player.h"

#include <iostream>
#include <utility>



LobbyPlayer::LobbyPlayer(Socket&& socket, const uint16_t id, Queue<LobbyCommand>& lobby_queue):
        protocol(std::move(socket)),
        player_id(id),
        lobby_receiver(protocol, lobby_queue),
        connected(true),
        playing(false),
        match_id(0) {}

bool LobbyPlayer::send_lobby_message(const LobbyMessage& msg) {
        protocol.send_lobby_message(msg);
        return true;
}

void LobbyPlayer::start() {
    lobby_receiver.start();
}


std::shared_ptr<Player> LobbyPlayer::start_game(Queue<Command>& game_queue) {
    //detengo el hilo para que el cliente no hable mas con el lobby
    playing = true;
    lobby_receiver.stop();
    lobby_receiver.join();
    connected = false;

    //en lugar de mover el protocolo podria crear uno nuevo exclusivo de Game
    std::shared_ptr<Player> player = std::make_shared<Player>(std::move(protocol), player_id, game_queue);
    return  player;

}


void LobbyPlayer::stop() {

    lobby_receiver.stop();
    lobby_receiver.join();
}

uint16_t LobbyPlayer::get_player_id() { return player_id; }

void LobbyPlayer::set_match_id(uint16_t id){
    match_id = id;
}

uint16_t LobbyPlayer::get_match_id() { return match_id; }

bool LobbyPlayer::is_connected() { return connected; }

//Queue<Message>& Player::get_message_queue() { return sender.get_queue(); }
