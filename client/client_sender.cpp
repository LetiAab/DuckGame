#include "client_sender.h"

#include <iostream>

#include "common/liberror.h"


ClientSender::ClientSender(ClientProtocol& protocol, uint16_t id): protocol(protocol), id(id), is_alive(true), command_queue() {}


void ClientSender::run() {
    try {

        while (is_alive) {

            //Este hilo que se encargue de mandar los mensajes al servidor
            //Por ahora no uso la queue, despues veo si la meto

            std::string input;
            std::getline(std::cin, input);


            if (input.empty() || input == "Exit") {
                break;
            }

            if (input == "5"){
                Command command;
                command.player_id = id;
                command.type = NEW_MATCH_CODE;

                if (protocol.send_command(command)){
                    std::cout << "Comando Enviado" << "\n";
                };
            }

            if (input == "6"){
                //este es para conectarte
                Command command;
                command.player_id = id;
                command.type = EXISTING_MATCH_CODE;

                if (protocol.send_command(command)){
                    std::cout << "Comando Enviado" << "\n";
                };
            }

            if (input == "9"){
                //este es para conectarte
                Command command;
                command.player_id = id;
                command.type = LIST_MATCH_AVAILABLE;

                if (protocol.send_command(command)){
                    std::cout << "Comando Enviado" << "\n";
                };
            }

            
            //Message msj = message_queue.pop();
            //protocol.send_data(msj);
        }

    } catch (const ClosedQueue& e) {
        is_alive = false;

    } catch (const LibError& e) {
        is_alive = false;

    } catch (const std::exception& e) {
        is_alive = false;
        std::cerr << "Exeption running the sender: " << e.what() << std::endl;
    }
}


bool ClientSender::is_running() { return is_alive; }

void ClientSender::stop() { is_alive = false; }
