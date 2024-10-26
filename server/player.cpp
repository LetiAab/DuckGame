#include "player.h"

#include <iostream>
#include <utility>



Player::Player(Socket&& socket, const uint16_t id, Queue<LobbyCommand>& lobby_queue):
        protocol(std::move(socket)),
        player_id(id),
        sender(protocol),
        //receiver(protocol),
        lobby_receiver(protocol, lobby_queue),
        connected(true),
        playing(false) {}

bool Player::send_lobby_message(const LobbyMessage& msg) {
        protocol.send_lobby_message(msg);
        return true;
}

void Player::start() {
    lobby_receiver.start();
}


void Player::start_playing() {
    //detengo el hilo para que el cliente no hable mas con el lobby
    lobby_receiver.stop();
    lobby_receiver.join();
    //receiver = Receiver(protocol,queue); //no puedo hacer esto, voy a tener que crear otra clase
    //receiver.start();
    sender.start();
    playing = true;

}

void Player::stop_playing() {
    connected = false;
    protocol.shutdown();

    //receiver.stop();
    sender.stop();

    sender.join();
    //receiver.join();
}

void Player::stop() {
    connected = false;
    protocol.shutdown();

    lobby_receiver.stop();
    lobby_receiver.join();
}

uint16_t Player::get_player_id() { return player_id; }

bool Player::is_connected() { return connected; }

//Queue<Message>& Player::get_message_queue() { return sender.get_queue(); }
