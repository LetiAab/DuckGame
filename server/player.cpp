#include "player.h"

#include <iostream>
#include <utility>


Player::Player(ServerProtocol&& prot, uint16_t id, Queue<std::shared_ptr<Executable>>& game_queue):
        protocol(std::move(prot)),
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
    if(receiver.is_running()){
        protocol.shutdown();
    }

    receiver.stop();
    sender.stop();
    sender.join();
    receiver.join();
    std::cout << "Player: libere mis recursos"  << std::endl;
}

uint16_t Player::get_player_id() { return player_id; }

void Player::set_player_id(uint16_t id){ id = id; }

bool Player::is_connected() { return connected; }

Queue<Message>& Player::get_message_queue() { return sender.get_queue(); }
