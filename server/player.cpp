#include "player.h"

#include <iostream>
#include <utility>



Player::Player(Socket&& socket, const int id, Queue<LobbyCommand>& lobby_queue):
        protocol(std::move(socket)),
        player_id(id),
        sender(protocol),
        receiver(protocol, lobby_queue),
        connected(true),
        in_match(false) {}

bool Player::send_lobby_message(const LobbyMessage& msg) {
        protocol.send_lobby_message(msg);
        return true;
}

void Player::start() {
    receiver.start();
}

void Player::start_playing() {
    //seteo nuevas queues al sender y receiver (?)
    //sender.start()
}

void Player::stop_playing() {
    connected = false;
    protocol.shutdown();

    receiver.stop();
    sender.stop();

    sender.join();
    receiver.join();
}

int Player::get_player_id() { return player_id; }

bool Player::is_connected() { return connected; }

//Queue<Message>& Player::get_message_queue() { return sender.get_queue(); }
