#include "client_sender.h"

#include <iostream>

#include "common/liberror.h"


ClientSender::ClientSender(ClientProtocol& protocol): protocol(protocol), is_alive(true), command_queue() {}


void ClientSender::run() {

    try {
        while (is_alive) {

            auto cmd = command_queue.pop();
            cmd->send_myself(protocol);

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

Queue<std::shared_ptr<Sendable>>& ClientSender::get_queue(){ return command_queue; }


bool ClientSender::is_running() { return is_alive; }

void ClientSender::stop() { is_alive = false; }
