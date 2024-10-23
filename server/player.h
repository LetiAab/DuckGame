#ifndef PLAYER_H
#define PLAYER_H
#include <cstdint>
#include <string>

#include "common/queue.h"
#include "common/lobby_message.h"
#include "common/lobby_command.h"

#include "server_protocol.h"
#include "receiver.h"
#include "sender.h"


class Player {
    
private:
    ServerProtocol protocol;
    const int player_id;
    Sender sender;
    Receiver receiver;
    bool connected;
    bool in_match;


public:
    Player(Socket&& socket, const int id, Queue<LobbyCommand>& lobby_queue);

    void start();

    bool send_lobby_message(const LobbyMessage& msj); 
    
    void start_playing();

    bool is_connected();

    void stop_playing();

    int get_player_id();

    //Queue<Message>& get_message_queue();
};

#endif
