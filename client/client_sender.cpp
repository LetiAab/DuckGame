#include "client_sender.h"

#include <iostream>

#include "common/liberror.h"


ClientSender::ClientSender(ClientProtocol& protocol): protocol(protocol), is_alive(true), command_queue() {}


void ClientSender::run() {
    try {
        while (is_alive) {
            auto cmd = command_queue.pop();
            protocol.send_command(cmd);
        }

    } catch (const ClosedQueue& e) {
        is_alive = false;
        std::cerr << "Sender: se cerro la command_queue"  << std::endl;
        //protocol.shutdown();
    } catch (const LibError& e) {
        is_alive = false;
        std::cerr << "Sender: se cerro el socket"  << std::endl;

    } catch (const std::exception& e) {
        is_alive = false;
        std::cerr << "Exeption running the sender: " << e.what() << std::endl;
    }
}

Queue<Command>& ClientSender::get_queue(){ return command_queue; }


bool ClientSender::is_running() { return is_alive; }

void ClientSender::stop() { is_alive = false; }
