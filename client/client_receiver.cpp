#include "client_receiver.h"

#include <iostream>

#include "common/liberror.h"


ClientReceiver::ClientReceiver(ClientProtocol& protocol, uint16_t id):
        protocol(protocol), id(id), is_alive(true), message_queue() {}


void ClientReceiver::run() {
    try {
        


        while (is_alive) {
            /*

            Por ahora no uso las queues pero las necesito

            Command cmd = protocol.get_command();
            command_queue.push(cmd);
            */

            Message message = protocol.recive_message();

            std::cout << "Recibo message del server" << "\n";
            
            std::cout << "Player id: " << message.player_id << "\n";
            std::cout << "Type: " << static_cast<int>(message.type) << "\n"; 
            

        }

    } catch (const ClosedQueue& e) {
        is_alive = false;

    } catch (const LibError& e) {
        is_alive = false;

    } catch (const std::exception& e) {
        is_alive = false;
        std::cerr << "Exeption running the receiver: " << e.what() << std::endl;
    }
}

bool ClientReceiver::is_running() { return is_alive; }

void ClientReceiver::stop() { is_alive = false; }
