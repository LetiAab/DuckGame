#include <cstdint>
#include <string>

#include "common/queue.h"
#include "common/thread.h"
#include "common/lobby_command.h"

#include "../server_protocol.h"

#ifndef LOBBY_RECEIVER_H
#define LOBBY_RECEIVER_H

class LobbyReceiver: public Thread {
private:
    ServerProtocol& protocol;
    bool is_alive;
    Queue<LobbyCommand>& lobby_queue;


public:
    LobbyReceiver(ServerProtocol& protocol, Queue<LobbyCommand>& lobby_queue);

    void run() override;

    bool is_running();

    void stop() override;
};

#endif
