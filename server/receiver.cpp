#include "receiver.h"

#include <iostream>

#include "common/liberror.h"


Receiver::Receiver(ServerProtocol& protocol, Queue<LobbyCommand>& lobby_queue):
        protocol(protocol), is_alive(true), lobby_queue(lobby_queue) {}


void Receiver::run() {
    try {
        while (is_alive) {

            LobbyCommand cmd = protocol.get_lobby_command();
            lobby_queue.push(cmd);
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

void Receiver::stop() { is_alive = false; }
