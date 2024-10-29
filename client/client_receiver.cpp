#include "client_receiver.h"

#include <iostream>

#include "common/liberror.h"


ClientReceiver::ClientReceiver(ClientProtocol& protocol, uint16_t id):
        protocol(protocol), id(id), is_alive(true), message_queue() {}

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
            /*

            Por ahora no uso las queues pero las necesito

            Command cmd = protocol.get_command();
            command_queue.push(cmd);
            */

            LobbyMessage lobbyMessage = protocol.recive_lobby_message();

            //cada vez que mando un mensaje el server me responde pero no se para que
            
            
            
            if (lobbyMessage.type == NEW_MATCH_CODE){
                            
                std::cout << "Partida creada con id: " << static_cast<int>(lobbyMessage.current_match_id) << "\n"; 

            }

            if (lobbyMessage.type == EXISTING_MATCH_CODE){
                std::cout << "Conectado a partida con id: " << static_cast<int>(lobbyMessage.current_match_id) << "\n"; 

            }

            if (lobbyMessage.type == START_MATCH_CODE){
                std::cout << "Partida iniciada con id: " << static_cast<int>(lobbyMessage.current_match_id) << "\n"; 
                
                LobbyCommand command2;
                command2.player_id = id;
                command2.type = LOBBY_STOP_CODE;
                command2.match_id = 0;

                if (protocol.send_lobby_command(command2)){
                    //std::cout << "Stop code" << "\n";
                    continue;
                };
            }

        
            std::cout << "\n";

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
