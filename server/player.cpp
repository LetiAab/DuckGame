#include "player.h"

#include <iostream>
#include <utility>


Player::Player(ServerProtocol&& protocol, const uint16_t id, Queue<Command>& game_queue):
        protocol(std::move(protocol)),
        player_id(id),
        sender(protocol),
        receiver(protocol, game_queue),
        connected(true),
        playing(false) {}



void Player::start_playing() {
    receiver.start();
    sender.start();
    playing = true;


}

void Player::stop_playing() {
    connected = false;
    protocol.shutdown();

    receiver.stop();
    sender.stop();

    sender.join();
    receiver.join();
}

uint16_t Player::get_player_id() { return player_id; }

bool Player::is_connected() { return connected; }

//Queue<Message>& Player::get_message_queue() { return sender.get_queue(); }
