#ifndef PLAYER_H
#define PLAYER_H
#include <cstdint>
#include <string>

#include "common/queue.h"
#include "common/lobby_message.h"
#include "common/lobby_command.h"
#include "common/command.h"

#include "server_protocol.h"
#include "receiver.h"
#include "sender.h"


class Player {
    
private:
    ServerProtocol protocol;
    const uint16_t player_id;
    Sender sender;
    Receiver receiver;
    bool connected;
    bool playing;


public:
    Player(ServerProtocol&& protocol, const uint16_t id, Queue<Command>& game_queue);

    void start_playing();

    bool is_connected();

    void stop_playing();

    uint16_t get_player_id();


    //Queue<Message>& get_message_queue();
};

#endif
