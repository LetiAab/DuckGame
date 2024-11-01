#include <cstdint>
#include <string>

#include "common/queue.h"
#include "common/thread.h"
#include "common/lobby_message.h"

#include "common/client_protocol.h"
#include "common/constants.h"

#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

class ClientReceiver: public Thread {
private:
    ClientProtocol& protocol;
    bool is_alive;
    Queue<LobbyMessage> message_queue;


public:
    ClientReceiver(ClientProtocol& protocol);

    void run() override;

    bool is_running();

    void stop() override;

    Queue<LobbyMessage>& get_queue();
};

#endif
