#include "client.h"


const int OK = 0;



Client::Client(const std::string& hostname, const std::string& port):
        protocol(Socket(hostname.c_str(), port.c_str())){}

int Client::start() {

    // primer mensaje de la conexion para saber mi id
    Message first_message = protocol.recive_message();

    std::cout << "First Message. Player id: " << first_message.player_id << "\n";
    std::cout << "First Message. Type: " << static_cast<int>(first_message.type) << "\n"; 
        
    //persisto mi id
    uint16_t id = first_message.player_id;

    //aca simplemente inicio los hilos sender y receiver
    sender = std::make_unique<ClientSender>(protocol, id);
    receiver = std::make_unique<ClientReceiver>(protocol, id);

    sender->start();
    receiver->start();

    sender->join();
    receiver->join();



    // protocol.close();
    return OK;
}
