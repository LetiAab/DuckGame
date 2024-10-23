#include "sender.h"

#include <iostream>

#include "common/liberror.h"


Sender::Sender(ServerProtocol& protocol): protocol(protocol), is_alive(true) {}


void Sender::run() {
    try {

        while (is_alive) {
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

//Queue<Message>& Sender::get_queue() { return message_queue; }

bool Sender::is_running() { return is_alive; }

void Sender::stop() { is_alive = false; }
