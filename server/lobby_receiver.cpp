#include "lobby_receiver.h"

#include <iostream>

#include "common/liberror.h"
#include "common/constants.h"


LobbyReceiver::LobbyReceiver(ServerProtocol& protocol, Queue<LobbyCommand>& lobby_queue):
        protocol(protocol), is_alive(true), lobby_queue(lobby_queue) {}


void LobbyReceiver::run() {
    try {
        while (is_alive) {

            LobbyCommand cmd = protocol.get_lobby_command();

            if (cmd.type == LOBBY_STOP_CODE) {
                is_alive = false;
                break;
            }

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

bool LobbyReceiver::is_running() { return is_alive; }

void LobbyReceiver::stop() { is_alive = false; }
