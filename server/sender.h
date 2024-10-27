#include <cstdint>
#include <string>


#include "common/queue.h"
#include "common/thread.h"
#include "common/message.h"

#include "server_protocol.h"

#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

class Sender: public Thread {
private:
    ServerProtocol& protocol;
    bool is_alive;
    Queue<Message> message_queue;


public:
    explicit Sender(ServerProtocol& protocol);

    void run() override;

    bool is_running();

    void stop() override;

    Queue<Message>& get_queue();
};

#endif
