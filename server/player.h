#ifndef PLAYER_H
#define PLAYER_H
#include <cstdint>
#include <string>

#include "../common/queue.h"
#include "../common/message.h"
#include "../common/command.h"
#include "executables/executable.h"

#include "server_protocol.h"
#include "receiver.h"
#include "sender.h"


class Player {
    
private:
    ServerProtocol protocol;
    uint16_t player_id;
    Sender sender;
    Receiver receiver;
    bool connected;
    bool playing;


public:
    Player(ServerProtocol&& protocol, uint16_t id, Queue<std::shared_ptr<Executable>>& game_queue);

    void start_playing();

    bool is_connected();

    void stop_playing();

    uint16_t get_player_id();

    void set_player_id(uint16_t id);

    Queue<Message>& get_message_queue();
};

#endif
