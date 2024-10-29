#include "client.h"


const int OK = 0;



Client::Client(const std::string& hostname, const std::string& port):
        protocol(Socket(hostname.c_str(), port.c_str())){}

void printExistingMatches(const std::vector<uint16_t>& existing_matches) {
    std::cout << "Ids de partidas disponibles: ";
    if (existing_matches.empty()) {
        std::cout << "No existing matches.";
    } else {
        for (size_t i = 0; i < existing_matches.size(); ++i) {
            std::cout << existing_matches[i];
            if (i < existing_matches.size() - 1) {
                std::cout << ", ";  
            }
        }
    }
    std::cout << "\n"; 
}

int Client::start() {

    // primer mensaje de la conexion para saber mi id
    LobbyMessage first_message = protocol.recive_lobby_message();

    std::cout << "First Message. Player id: " << first_message.player_id << "\n";
    printExistingMatches(first_message.existing_matches);

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

