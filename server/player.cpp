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
    std::cout << "Player: voy a cerrar el protocolo"  << std::endl;
        protocol.shutdown();
    }
    std::cout << "Player: voy a detener el receiver"  << std::endl;
    receiver.stop();
    std::cout << "Player: voy a detener el sender"  << std::endl;
    sender.stop();
    std::cout << "Player: voy a joinear el sender"  << std::endl;
    sender.join();
    std::cout << "Player: voy a joinear el receiver"  << std::endl;
    receiver.join();
    std::cout << "Player: listo"  << std::endl;
}

uint16_t Player::get_player_id() { return player_id; }

void Player::set_player_id(uint16_t id){ id = id; }

bool Player::is_connected() { return connected; }

Queue<Message>& Player::get_message_queue() { return sender.get_queue(); }
