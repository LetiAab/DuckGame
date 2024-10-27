#include "lobby_player.h"

#include <iostream>
#include <utility>



LobbyPlayer::LobbyPlayer(Socket&& socket, const uint16_t id, Queue<LobbyCommand>& lobby_queue):
        protocol(std::move(socket)),
        player_id(id),
        lobby_receiver(protocol, lobby_queue),
        connected(true),
        playing(false) {}

bool LobbyPlayer::send_lobby_message(const LobbyMessage& msg) {
        protocol.send_lobby_message(msg);
        return true;
}

void LobbyPlayer::start() {
    lobby_receiver.start();
}


void LobbyPlayer::start_playing() {
    //detengo el hilo para que el cliente no hable mas con el lobby
    lobby_receiver.stop();
    lobby_receiver.join();
    //receiver = Receiver(protocol,queue); //no puedo hacer esto, voy a tener que crear otra clase
    //receiver.start();
    playing = true;

}

void LobbyPlayer::stop_playing() {
    connected = false;
    protocol.shutdown();

}

void LobbyPlayer::stop() {
    connected = false;
    protocol.shutdown();

    lobby_receiver.stop();
    lobby_receiver.join();
}

uint16_t LobbyPlayer::get_player_id() { return player_id; }

bool LobbyPlayer::is_connected() { return connected; }

//Queue<Message>& Player::get_message_queue() { return sender.get_queue(); }
