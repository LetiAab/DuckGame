#include "receiver.h"

#include <iostream>

#include "common/liberror.h"


Receiver::Receiver(ServerProtocol& protocol, Queue<Command>& queue):
        protocol(protocol), is_alive(true), command_queue(queue) {}


void Receiver::run() {
    try {
        while (is_alive) {
            /*
            Command cmd = protocol.get_command();
            command_queue.push(cmd);
            */
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

bool Receiver::is_running() { return is_alive; }

//void Receiver::set_queue(Queue<Command>& queue) { command_queue = queue; }

void Receiver::stop() { is_alive = false; }
