#include "client_receiver.h"

#include <iostream>

#include "common/liberror.h"


ClientReceiver::ClientReceiver(ClientProtocol& protocol):
        protocol(protocol), is_alive(true), message_queue() {}


void ClientReceiver::run() {
    try {
        while (is_alive) {

            Message msg = protocol.receive_message();
            message_queue.push(msg);
        }

    } catch (const ClosedQueue& e) {
        is_alive = false;

    } catch (const LibError& e) {
        is_alive = false;
        message_queue.close();

    } catch (const std::exception& e) {
        is_alive = false;
        message_queue.close();
        std::cerr << "Exception running the receiver: " << e.what() << std::endl;
    }
}
Queue<Message>& ClientReceiver::get_queue(){ return message_queue; }

bool ClientReceiver::is_running() { return is_alive; }

void ClientReceiver::stop() { is_alive = false; }
