#include <cstdint>
#include <string>

#include "common/queue.h"
#include "common/thread.h"
#include "common/command.h"

#include "client_protocol.h"
#include "common/constants.h"


#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

class ClientSender: public Thread {
private:
    ClientProtocol& protocol;
    bool is_alive;
    Queue<Command> command_queue;


public:
    explicit ClientSender(ClientProtocol& protocol);

    void run() override;

    bool is_running();

    void stop() override;

    Queue<Command>& get_queue();
};

#endif
