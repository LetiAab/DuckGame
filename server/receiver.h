#include <cstdint>
#include <string>

#include "common/queue.h"
#include "common/thread.h"
#include "common/command.h"
#include "common/executable.h"

#include "server_protocol.h"

#ifndef RECEIVER_H
#define RECEIVER_H

class Receiver: public Thread {
private:
    ServerProtocol& protocol;
    bool is_alive;
    Queue<std::shared_ptr<Executable>>& game_queue;


public:
    Receiver(ServerProtocol& protocol, Queue<std::shared_ptr<Executable>>& game_queue);

    void run() override;

    bool is_running();

    void stop() override;
};

#endif
