#include <cstdint>
#include <string>


#include "common/queue.h"
#include "common/thread.h"
#include  "common/command.h"
#include  "common/message.h"


#include "client_protocol.h"
#include "common/constants.h"


#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

class ClientSender: public Thread {
private:
    ClientProtocol& protocol;
    uint16_t id;
    bool is_alive;
    Queue<Command> command_queue;


public:
    explicit ClientSender(ClientProtocol& protocol, uint16_t id);

    void run() override;

    bool is_running();

    void stop() override;

    Queue<Message>& get_queue();
};

#endif
