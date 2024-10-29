#include <cstdint>
#include <string>

#include "common/queue.h"
#include "common/thread.h"
#include "common/message.h"

#include "client_protocol.h"

#include "common/constants.h"

#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

class ClientReceiver: public Thread {
private:
    ClientProtocol& protocol;
    uint16_t id;
    bool is_alive;
    Queue<Message> message_queue;


public:
    ClientReceiver(ClientProtocol& protocol, uint16_t id);

    void run() override;

    bool is_running();

    void stop() override;
};

#endif
