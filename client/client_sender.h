#include <cstdint>
#include <string>

#include "common/queue.h"
#include "common/thread.h"
#include "common/lobby_command.h"
#include "common/sendable.h"

#include "common/client_protocol.h"
#include "common/constants.h"


#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

class ClientSender: public Thread {
private:
    ClientProtocol& protocol;
    bool is_alive;
    Queue<std::shared_ptr<Sendable>> command_queue;


public:
    explicit ClientSender(ClientProtocol& protocol);

    void run() override;

    bool is_running();

    void stop() override;

    Queue<std::shared_ptr<Sendable>>& get_queue();
};

#endif
