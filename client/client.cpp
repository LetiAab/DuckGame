#include "client.h"

#include "common/message.h"
#include "common/queue.h"

Client::Client(const std::string& hostname, const std::string& port):
        protocol(Socket(hostname.c_str(), port.c_str())){}

int Client::start(){
    // primer mensaje de la conexion para saber mi id
    Message first_message = protocol.receive_message();

    //persisto mi id del lobby
    uint16_t lobby_id = first_message.player_id;

    sender = std::make_unique<ClientSender>(protocol);
    receiver = std::make_unique<ClientReceiver>(protocol);

    //inicio los hilos
    sender->start();
    receiver->start();

    sdl_handler = std::make_unique<SDLHandler>();
    int result = sdl_handler->run(lobby_id, sender->get_queue(), receiver->get_queue(), protocol);

    close();

    return result;
}

void Client::close() {
    std::cout << "Cerrando socket..." << std::endl;
    protocol.shutdown();
    std::cout << "Cerrando receiver..." << std::endl;
    receiver->stop();
    receiver->join();
    std::cout << "Cerrando sender..." << std::endl;
    sender->stop();
    sender->join();
    std::cout << "Cliente cerrado..." << std::endl;
}