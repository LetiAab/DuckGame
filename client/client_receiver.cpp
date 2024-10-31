#include "client_receiver.h"

#include <iostream>

#include "common/liberror.h"


ClientReceiver::ClientReceiver(ClientProtocol& protocol):
        protocol(protocol), is_alive(true), message_queue() {}

void printExistingMatches2(const std::vector<uint16_t>& existing_matches) {
    std::cout << "Lobby Message. Existing matches: ";
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


void ClientReceiver::run() {
    try {
        while (is_alive) {

            //por ahora recibo solo LobbyMessage
            LobbyMessage msg = protocol.recive_lobby_message();
            message_queue.push(msg);

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
Queue<LobbyMessage>& ClientReceiver::get_queue(){ return message_queue; }

bool ClientReceiver::is_running() { return is_alive; }

void ClientReceiver::stop() { is_alive = false; }
