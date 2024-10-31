#include "client_sender.h"

#include <iostream>

#include "common/liberror.h"


ClientSender::ClientSender(ClientProtocol& protocol): protocol(protocol), is_alive(true), command_queue() {}


void ClientSender::run() {

    try {
        while (is_alive) {

            //Este hilo que se encarga de mandar los comandos al servidor
            //Por ahora solo funciona con LobbyCommand
            //despues se cambiara por command u otro tipo mas amplio

            LobbyCommand cmd = command_queue.pop();
            protocol.send_lobby_command(cmd);
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

Queue<LobbyCommand>& ClientSender::get_queue(){ return command_queue; }


bool ClientSender::is_running() { return is_alive; }

void ClientSender::stop() { is_alive = false; }
